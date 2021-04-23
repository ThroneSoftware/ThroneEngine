#include <Graphics/GraphicsInstance.h>

#include <Graphics/VulkanContext.h>
#include <Graphics/VulkanContextFactory.h>
#include <Graphics/VulkanWrappers/Commands/CommandBufferRecordScope.h>
#include <Graphics/VulkanWrappers/Commands/CommandPool.h>
#include <Graphics/VulkanWrappers/FrameBuffer.h>
#include <Graphics/VulkanWrappers/Memory/Image.h>
#include <Graphics/VulkanWrappers/RenderPass.h>
#include <Graphics/VulkanWrappers/Syncronization/FencePool.h>
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <gsl/gsl>

#include <iostream>

trg::Image makeDepthImage(trg::VulkanContext& vkContext)
{
	auto depthImage = trg::Image(vkContext.m_device,
								 vk::ImageType::e2D,
								 vk::Format::eD32Sfloat,
								 vk::Extent3D(vkContext.m_swapchainExtent, 1),
								 1,
								 1,
								 vk::SampleCountFlagBits::e1,
								 vk::ImageUsageFlagBits::eDepthStencilAttachment,
								 vk::ImageLayout::eUndefined);

	depthImage.addImageView(vk::ImageAspectFlagBits::eDepth, vk::ImageViewType::e2D, vk::Format::eD32Sfloat, 0, 1);

	return depthImage;
}

trg::RenderPass makeRenderPass(trg::VulkanContext& vkContext)
{
	return trg::RenderPass(vkContext.m_device, vkContext.m_swapchain.getFormat());
}

trg::FrameBuffer
	makeFrameBuffer(trg::VulkanContext& vkContext, trg::ImageView& swapchainImageView, trg::Image& depthImage, trg::RenderPass& renderPass)
{
	std::vector<vk::ImageView> attachments = {*swapchainImageView, *depthImage.getImageView()};
	return trg::FrameBuffer(vkContext.m_device, renderPass, attachments, vkContext.m_swapchainExtent, 1);
}

trg::Fence makeFence(trg::VulkanContext& vkContext)
{
	return trg::Fence(vkContext.m_device);
}

trg::Semaphore makeSemaphore(trg::VulkanContext& vkContext)
{
	return trg::Semaphore(vkContext.m_device);
}

class FrameContext
{
public:
	FrameContext(trg::GraphicsInstance& graphicsInstance, trg::CommandBuffer& commandBuffer, trg::ImageView& swapchainImageView)
	  : m_graphicsInstance(graphicsInstance)
	  , m_vkContext(m_graphicsInstance.vulkanContext())
	  , m_commandBuffer(commandBuffer)
	  , m_swapchainImageView(swapchainImageView)
	  , m_depthImage(makeDepthImage(m_vkContext))
	  , m_renderPass(makeRenderPass(m_vkContext))
	  , m_frameBuffer(makeFrameBuffer(m_vkContext, m_swapchainImageView, m_depthImage, m_renderPass))
	  , m_acquireNextImageSemaphore(makeSemaphore(m_vkContext))
	  , m_submitCommandBufferFinishedFence(makeFence(m_vkContext))
	  , m_submitCommandBufferFinishedSemaphore(makeSemaphore(m_vkContext))
	{
	}

	void renderFrame(glm::vec3 clearColor)
	{
		m_submitCommandBufferFinishedFence.wait();
		m_submitCommandBufferFinishedFence.reset();

		auto imageIndex = m_vkContext.m_device.acquireNextImageKHR(m_vkContext.m_swapchain.getSwapchain(),
																   std::numeric_limits<std::uint64_t>::max(),
																   *m_acquireNextImageSemaphore,
																   vk::Fence());

		{
			auto cmdScope = trg::CommandBufferRecordScope(m_commandBuffer, vk::CommandBufferUsageFlagBits::eOneTimeSubmit);


			vk::SubpassContents subpassContents = vk::SubpassContents::eInline;

			vk::ClearValue vkClearColor = vk::ClearValue(vk::ClearColorValue(std::array{clearColor.r, clearColor.g, clearColor.b, 1.0f}));
			vk::ClearValue vlClearDepth = vk::ClearValue(vk::ClearDepthStencilValue(1.0f));

			auto clearValues = std::vector<vk::ClearValue>{vkClearColor, vlClearDepth};
			auto beginInfo =
				vk::RenderPassBeginInfo(*m_renderPass, *m_frameBuffer, vk::Rect2D({0, 0}, m_vkContext.m_swapchainExtent), clearValues);

			m_commandBuffer->beginRenderPass(beginInfo, subpassContents);
			m_commandBuffer->endRenderPass();
		}

		std::vector submitWaitSemaphores = {*m_acquireNextImageSemaphore};
		m_vkContext.m_graphicsQueue.submitCommandBuffer(submitWaitSemaphores,
														{vk::PipelineStageFlagBits::eAllGraphics},
														m_commandBuffer,
														std::span(&m_submitCommandBufferFinishedSemaphore, 1),
														m_submitCommandBufferFinishedFence);

		std::vector presentWaitSemaphores = {*m_submitCommandBufferFinishedSemaphore};
		m_graphicsInstance.present(imageIndex.value, presentWaitSemaphores);
	}

private:
	// Order of variables is important for initialization.

	trg::GraphicsInstance& m_graphicsInstance;
	trg::VulkanContext& m_vkContext;

	trg::CommandBuffer& m_commandBuffer;

	trg::ImageView& m_swapchainImageView;

	trg::Image m_depthImage;

	trg::RenderPass m_renderPass;
	trg::FrameBuffer m_frameBuffer;

	trg::Semaphore m_acquireNextImageSemaphore;

	trg::Fence m_submitCommandBufferFinishedFence;
	trg::Semaphore m_submitCommandBufferFinishedSemaphore;
};

int main()
{
	auto instance = trg::GraphicsInstance(std::make_unique<trg::GraphicsContext>(trg::VulkanContextFactory()));

	auto swapchainImageViews = instance.vulkanContext().m_swapchain.getImageViews();
	auto frameContextCount = swapchainImageViews.size();

	auto commandBuffers = trg::CommandPool(instance.vulkanContext().m_device,
										   instance.vulkanContext().m_graphicsQueue,
										   vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
										   gsl::narrow<int>(frameContextCount),
										   vk::CommandBufferLevel::ePrimary);

	std::vector<FrameContext> frameContexts;
	frameContexts.reserve(frameContextCount);

	for(size_t i = 0; i < frameContextCount; ++i)
	{
		frameContexts.emplace_back(instance, commandBuffers.getAll()[i], swapchainImageViews[i]);
	}

	auto clearColors = std::array<glm::vec3, 6>{glm::vec3{1.0f, 0.0f, 0.0f},
												glm::vec3{1.0f, 1.0f, 0.0f},
												glm::vec3{0.0f, 1.0f, 0.0f},
												glm::vec3{0.0f, 1.0f, 1.0f},
												glm::vec3{0.0f, 0.0f, 1.0f},
												glm::vec3{1.0f, 0.0f, 1.0f}};

	int previousId = gsl::narrow<int>(clearColors.size() - 1);
	int currendId = 0;
	auto previousColor = clearColors[previousId];
	auto currentColor = clearColors[currendId];
	float sinValue = -glm::pi<float>() / 2.0f;
	float deltaTime = 0;
	float colorSpeed = 0.001f;

	uint64_t frameId = 0;
	while(true)
	{
		auto begin = std::chrono::steady_clock::now();

		auto frameIndex = frameId % frameContextCount;

		auto lerpValue = (std::sin(sinValue) + 1) / 2.0f;

		if(glm::epsilonEqual(lerpValue, 1.0f, glm::epsilon<float>()))
		{
			sinValue = -glm::pi<float>() / 2.0f;
			lerpValue = (std::sin(sinValue) + 1) / 2.0f;

			previousId = currendId;
			currendId = (currendId + 1) % (clearColors.size() - 1);

			previousColor = clearColors[previousId];
			currentColor = clearColors[currendId];
		}

		glm::vec3 clearColor = glm::lerp(previousColor, currentColor, lerpValue);

		FrameContext& frameContext = frameContexts[frameIndex];
		frameContext.renderFrame(clearColor);

		frameId++;

		auto end = std::chrono::steady_clock::now();

		deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - begin).count();

		sinValue += colorSpeed * deltaTime;
	}
}