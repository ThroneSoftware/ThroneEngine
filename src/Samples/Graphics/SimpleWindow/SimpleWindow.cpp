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

	// missing arguments
	//trg::Image depthImage = trg::Image(device);
	// init view

	trg::RenderPass renderPass = trg::RenderPass(device, vkContext.m_swapchain.getFormat());

	//trg::FrameBuffer frameBuffer =
	//	trg::FrameBuffer(device, renderPass, std::span(&depthImage.getImageView(), 1), vkContext.m_swapchainExtent, 1);

	trg::CommandPool commandBuffers = trg::CommandPool(device,
													   vkContext.m_graphicsQueue,
													   vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
													   2,
													   vk::CommandBufferLevel::ePrimary);

	trg::FencePool commandBufferFences = trg::FencePool(device, swapchainCount);

	trg::SemaphorePool renderFinishedSemaphores = trg::SemaphorePool(device, swapchainCount);

	uint64_t frameId = 0;
	while(true)
	{
		auto frameIndex = frameId % swapchainCount;

		auto& acquireNextImageSemaphore = vkContext.m_acquireNextImageSemaphores.getAll()[frameIndex];
		auto imageIndex = vkContext.m_device.acquireNextImageKHR(vkContext.m_swapchain.getSwapchain(),
																 std::numeric_limits<std::uint64_t>::max(),
																 *acquireNextImageSemaphore,
																 vk::Fence());

		auto& fence = commandBufferFences.getAll()[frameIndex];
		fence.wait();
		fence.reset();

		auto& commandBuffer = commandBuffers.getAll()[frameIndex];

		{
			auto cmdScope = trg::CommandBufferRecordScope(commandBuffer, vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

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