#include "ColorCycle.h"

#include <Graphics/GraphicsInstance.h>

#include <Graphics/VulkanContext.h>
#include <Graphics/VulkanContextFactory.h>
#include <Graphics/VulkanWrappers/Commands/CommandBufferRecordScope.h>
#include <Graphics/VulkanWrappers/Commands/CommandPool.h>
#include <Graphics/VulkanWrappers/Commands/RenderPassRecordScope.h>
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
	FrameContext(trg::GraphicsInstance& graphicsInstance,
				 trg::CommandBuffer& commandBuffer,
				 trg::ImageView& swapchainImageView,
				 trg::RenderPass& renderPass)
	  : m_graphicsInstance(graphicsInstance)
	  , m_vkContext(m_graphicsInstance.vulkanContext())
	  , m_commandBuffer(commandBuffer)
	  , m_swapchainImageView(swapchainImageView)
	  , m_renderPass(renderPass)
	  , m_depthImage(makeDepthImage(m_vkContext))
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

			auto renderPassScope = trg::RenderPassRecordScope(m_commandBuffer, m_vkContext, m_renderPass, m_frameBuffer, clearColor);
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
	// Order of variables is important for member initializer list.

	trg::GraphicsInstance& m_graphicsInstance;
	trg::VulkanContext& m_vkContext;

	trg::CommandBuffer& m_commandBuffer;

	trg::ImageView& m_swapchainImageView;

	trg::RenderPass& m_renderPass;

	trg::Image m_depthImage;

	trg::FrameBuffer m_frameBuffer;

	trg::Semaphore m_acquireNextImageSemaphore;

	trg::Fence m_submitCommandBufferFinishedFence;
	trg::Semaphore m_submitCommandBufferFinishedSemaphore;
};

int main()
{
	auto instance = trg::GraphicsInstance(std::make_unique<trg::GraphicsContext>(trg::VulkanContextFactory()));

	auto frameContextCount = instance.vulkanContext().m_swapchain.getImageViews().size();

	auto commandBuffers = trg::CommandPool(instance.vulkanContext().m_device,
										   instance.vulkanContext().m_graphicsQueue,
										   vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
										   gsl::narrow<int>(frameContextCount),
										   vk::CommandBufferLevel::ePrimary);

	auto renderPass = makeRenderPass(instance.vulkanContext());

	std::vector<FrameContext> frameContexts;
	frameContexts.reserve(frameContextCount);

	for(size_t i = 0; i < frameContextCount; ++i)
	{
		frameContexts.emplace_back(instance,
								   commandBuffers.getAll()[i],
								   instance.vulkanContext().m_swapchain.getImageViews()[i],
								   renderPass);
	}

	float deltaTime = 0;

	trg::ColorCycle colorCycle;

	uint64_t frameId = 0;
	while(!instance.windowShouldClose())
	{
		auto begin = std::chrono::steady_clock::now();

		instance.processGLFWEvents();

		if(bool expected = true; instance.vulkanContext().hasWindowResizeEvent.compare_exchange_strong(expected, false))
		{
			instance.vulkanContext().m_device.waitIdle();

			frameContexts.clear();
			for(size_t i = 0; i < frameContextCount; ++i)
			{
				frameContexts.emplace_back(instance,
										   commandBuffers.getAll()[i],
										   instance.vulkanContext().m_swapchain.getImageViews()[i],
										   renderPass);
			}

			// very important
			// Reset frameId to render to the correct frameBuffer after recreation.
			// Without this, we sometime get validation errors because 
			// frameId % frameContextCount is not equal to the result of acquireNextImageKHR.
			frameId = 0;
		}

		auto clearColor = colorCycle.getClearColor(deltaTime);

		auto frameIndex = frameId % frameContextCount;
		FrameContext& frameContext = frameContexts[frameIndex];
		frameContext.renderFrame(clearColor);

		frameId++;

		auto end = std::chrono::steady_clock::now();

		deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - begin).count();
	}

	instance.vulkanContext().m_device.waitIdle();
}