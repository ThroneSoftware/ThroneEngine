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

	constexpr auto swapchaiNCount = 2;

	// missing arguments
	//trg::Image depthImage = trg::Image(device);
	// init view

	//trg::RenderPass renderPass = trg::RenderPass(device, vkContext.m_swapchain.getFormat());

	//trg::FrameBuffer frameBuffer =
	//	trg::FrameBuffer(device, renderPass, std::span(&depthImage.getImageView(), 1), vkContext.m_swapchainExtent, 1);

	trg::SemaphorePool waitSemaphores(device);

	trg::CommandPool commandBuffers = trg::CommandPool(device,
													   vkContext.m_graphicsQueue,
													   vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
													   2,
													   vk::CommandBufferLevel::ePrimary);

	trg::FencePool commandBufferFences = trg::FencePool(device, swapchaiNCount);

	uint64_t frameId = 0;
	while(true)
	{
		auto frameIndex = frameId % swapchaiNCount;
		auto& fence = commandBufferFences.getAll()[frameIndex];
		fence.wait();

		auto& commandBuffer = commandBuffers.getAll()[frameIndex];

		{
			trg::CommandBufferRecordScope scope =
				trg::CommandBufferRecordScope(commandBuffer, vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

			//commandBuffer->beginRenderPass();
			//commandBuffer->endRenderPass();
		}

		fence.reset();
		vkContext.m_graphicsQueue.submitCommandBuffer(waitSemaphores, {}, commandBuffer, vkContext.m_presentWaitSemaphores, fence);

		instance.present();

		frameId++;
	}
}