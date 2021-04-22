#include <Graphics/GraphicsInstance.h>

#include <Graphics/VulkanContext.h>
#include <Graphics/VulkanContextFactory.h>
#include <Graphics/VulkanWrappers/Commands/CommandBufferRecordScope.h>
#include <Graphics/VulkanWrappers/Commands/CommandPool.h>
#include <Graphics/VulkanWrappers/FrameBuffer.h>
#include <Graphics/VulkanWrappers/Memory/Image.h>
#include <Graphics/VulkanWrappers/RenderPass.h>
#include <Graphics/VulkanWrappers/Syncronization/FencePool.h>

int main()
{
	auto instance = trg::GraphicsInstance(std::make_unique<trg::GraphicsContext>(trg::VulkanContextFactory()));
	auto& vkContext = instance.vulkanContext();
	auto& device = vkContext.m_device;

	constexpr auto swapchainCount = 2;

	auto depthImage = trg::Image(device,
								 vk::ImageType::e2D,
								 vk::Format::eD32Sfloat,
								 vk::Extent3D(vkContext.m_swapchainExtent, 1),
								 1,
								 1,
								 vk::SampleCountFlagBits::e1,
								 vk::ImageUsageFlagBits::eDepthStencilAttachment,
								 vk::ImageLayout::eUndefined);
	depthImage.addImageView(vk::ImageAspectFlagBits::eDepth, vk::ImageViewType::e2D, vk::Format::eD32Sfloat, 0, 1);

	auto renderPass = trg::RenderPass(device, vkContext.m_swapchain.getFormat());

	auto imageViews = vkContext.m_swapchain.getImageViews();
	std::vector<vk::ImageView> attachments = {*imageViews[0], *depthImage.getImageView()};
	auto frameBuffer = trg::FrameBuffer(device, renderPass, attachments, vkContext.m_swapchainExtent, 1);

	auto commandBuffers = trg::CommandPool(device,
										   vkContext.m_graphicsQueue,
										   vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
										   2,
										   vk::CommandBufferLevel::ePrimary);

	auto commandBufferFences = trg::FencePool(device, swapchainCount);

	auto renderFinishedSemaphores = trg::SemaphorePool(device, swapchainCount);

	uint64_t frameId = 0;
	while(true)
	{
		auto frameIndex = frameId % swapchainCount;

		auto& fence = commandBufferFences.getAll()[frameIndex];
		fence.wait();
		fence.reset();

		auto& acquireNextImageSemaphore = vkContext.m_acquireNextImageSemaphores.getAll()[frameIndex];
		auto imageIndex = vkContext.m_device.acquireNextImageKHR(vkContext.m_swapchain.getSwapchain(),
																 std::numeric_limits<std::uint64_t>::max(),
																 *acquireNextImageSemaphore,
																 vk::Fence());

		auto& commandBuffer = commandBuffers.getAll()[frameIndex];

		{
			auto cmdScope = trg::CommandBufferRecordScope(commandBuffer, vk::CommandBufferUsageFlagBits::eOneTimeSubmit);


			vk::SubpassContents subpassContents = vk::SubpassContents::eInline;

			vk::ClearValue clearColor = vk::ClearValue(vk::ClearColorValue(std::array{0.0f, 0.0f, 0.0f, 1.0f}));
			vk::ClearValue clearDepth = vk::ClearValue(vk::ClearDepthStencilValue(1.0f));

			auto clearValues = std::vector<vk::ClearValue>{clearColor, clearDepth};
			auto beginInfo =
				vk::RenderPassBeginInfo(*renderPass, *frameBuffer, vk::Rect2D({0, 0}, vkContext.m_swapchainExtent), clearValues);

			commandBuffer->beginRenderPass(beginInfo, subpassContents);
			commandBuffer->endRenderPass();
			//commandBuffer->beginRenderPass();
			//commandBuffer->endRenderPass();
		}

		std::vector submitWaitSemaphores = {*acquireNextImageSemaphore};
		vkContext.m_graphicsQueue.submitCommandBuffer(submitWaitSemaphores,
													  {vk::PipelineStageFlagBits::eAllGraphics},
													  commandBuffer,
													  std::span(&renderFinishedSemaphores.getAll()[frameIndex], 1),
													  fence);

		std::vector presentWaitSemaphores = {*renderFinishedSemaphores.getAll()[frameIndex]};
		instance.present(imageIndex.value, presentWaitSemaphores);

		frameId++;
	}
}