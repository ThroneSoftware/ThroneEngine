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

trg::vkwrappers::Image makeDepthImage(trg::VulkanContext& vkContext)
{
	auto depthImage = trg::vkwrappers::Image(vkContext.m_device,
											 vk::ImageType::e2D,
											 vk::Format::eD32Sfloat,
											 vk::Extent3D(vkContext.m_swapchainExtent, 1),
											 1,
											 1,
											 vk::SampleCountFlagBits::e1,
											 vk::ImageTiling::eOptimal,
											 vk::ImageUsageFlagBits::eDepthStencilAttachment,
											 vk::ImageLayout::eUndefined,
											 vma::MemoryUsage::eGpuOnly);

	depthImage.addImageView(vk::ImageAspectFlagBits::eDepth, vk::ImageViewType::e2D, vk::Format::eD32Sfloat, 0, 1);

	return depthImage;
}

trg::vkwrappers::RenderPass makeRenderPass(trg::VulkanContext& vkContext)
{
	return trg::vkwrappers::RenderPass(vkContext.m_device, vkContext.m_swapchain.getFormat());
}

trg::vkwrappers::FrameBuffer makeFrameBuffer(trg::VulkanContext& vkContext,
											 trg::vkwrappers::ImageView& swapchainImageView,
											 trg::vkwrappers::Image& depthImage,
											 trg::vkwrappers::RenderPass& renderPass)
{
	std::vector<vk::ImageView> attachments = {*swapchainImageView, *depthImage.getImageView()};
	return trg::vkwrappers::FrameBuffer(vkContext.m_device, renderPass, attachments, vkContext.m_swapchainExtent, 1);
}

trg::vkwrappers::Fence makeFence(trg::VulkanContext& vkContext)
{
	return trg::vkwrappers::Fence(vkContext.m_device);
}

trg::vkwrappers::Semaphore makeSemaphore(trg::VulkanContext& vkContext)
{
	return trg::vkwrappers::Semaphore(vkContext.m_device);
}

class FrameContext
{
public:
	FrameContext(trg::GraphicsInstance& graphicsInstance,
				 trg::vkwrappers::CommandBuffer& commandBuffer,
				 trg::vkwrappers::ImageView& swapchainImageView,
				 trg::vkwrappers::RenderPass& renderPass)
	  : m_graphicsInstance(graphicsInstance)
	  , m_vkContext(m_graphicsInstance.vulkanContext())
	  , m_commandBuffer(commandBuffer)
	  , m_swapchainImageView(swapchainImageView)
	  , m_renderPass(renderPass)
	  , m_depthImage(makeDepthImage(m_vkContext))
	  , m_frameBuffer(makeFrameBuffer(m_vkContext, m_swapchainImageView, m_depthImage, m_renderPass))
	  , m_submitCommandBufferFinishedFence(makeFence(m_vkContext))
	  , m_submitCommandBufferFinishedSemaphore(makeSemaphore(m_vkContext))
	{
	}

	void renderFrame(glm::vec3 clearColor, std::uint32_t imageIndex, trg::vkwrappers::Semaphore& acquireNextImageSemaphore)
	{
		m_submitCommandBufferFinishedFence.wait();
		m_submitCommandBufferFinishedFence.reset();

		{
			auto cmdScope = trg::vkwrappers::CommandBufferRecordScope(m_commandBuffer, vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

			auto renderPassScope =
				trg::vkwrappers::RenderPassRecordScope(m_commandBuffer, m_vkContext, m_renderPass, m_frameBuffer, clearColor);
		}

		std::vector submitWaitSemaphores = {*acquireNextImageSemaphore};
		m_vkContext.m_graphicsQueue.submitCommandBuffer(submitWaitSemaphores,
														{vk::PipelineStageFlagBits::eAllGraphics},
														m_commandBuffer,
														std::span(&m_submitCommandBufferFinishedSemaphore, 1),
														m_submitCommandBufferFinishedFence);

		std::vector presentWaitSemaphores = {*m_submitCommandBufferFinishedSemaphore};
		m_graphicsInstance.present(imageIndex, presentWaitSemaphores);
	}

private:
	// Order of variables is important for member initializer list.

	trg::GraphicsInstance& m_graphicsInstance;
	trg::VulkanContext& m_vkContext;

	trg::vkwrappers::CommandBuffer& m_commandBuffer;

	trg::vkwrappers::ImageView& m_swapchainImageView;

	trg::vkwrappers::RenderPass& m_renderPass;

	trg::vkwrappers::Image m_depthImage;

	trg::vkwrappers::FrameBuffer m_frameBuffer;

	trg::vkwrappers::Fence m_submitCommandBufferFinishedFence;
	trg::vkwrappers::Semaphore m_submitCommandBufferFinishedSemaphore;
};

auto makeFrameContexts(std::size_t frameContextCount,
					   trg::GraphicsInstance& instance,
					   trg::vkwrappers::CommandPool& commandBuffers,
					   trg::vkwrappers::RenderPass& renderPass)
{
	std::vector<FrameContext> frameContexts;
	frameContexts.reserve(frameContextCount);

	for(size_t i = 0; i < frameContextCount; ++i)
	{
		frameContexts.emplace_back(instance,
								   commandBuffers.getAll()[i],
								   instance.vulkanContext().m_swapchain.getImageViews()[i],
								   renderPass);
	}

	return frameContexts;
}

int main()
{
	auto instance = trg::GraphicsInstance(std::make_unique<trg::GraphicsContext>(trg::VulkanContextFactory()));
	auto& vkContext = instance.vulkanContext();

	auto frameContextCount = vkContext.m_swapchain.getImageViews().size();

	auto commandBuffers = trg::vkwrappers::CommandPool(vkContext.m_device,
													   vkContext.m_graphicsQueue,
													   vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
													   frameContextCount,
													   vk::CommandBufferLevel::ePrimary);

	auto renderPass = makeRenderPass(vkContext);

	auto acquireNextImageSemaphores = trg::vkwrappers::SemaphorePool(vkContext.m_device, frameContextCount);

	std::vector<FrameContext> frameContexts = makeFrameContexts(frameContextCount, instance, commandBuffers, renderPass);

	float deltaTime = 0;

	trg::ColorCycle colorCycle;

	uint64_t frameId = 0;
	while(!instance.windowShouldClose())
	{
		auto currentResourceIndex = frameId % frameContextCount;

		auto begin = std::chrono::steady_clock::now();

		instance.processWindowEvents();

		if(!vkContext.windowMinimized)
		{
			if(bool expected = true; vkContext.hasWindowResizeEvent.compare_exchange_strong(expected, false))
			{
				vkContext.m_device.waitIdle();

				frameContexts = makeFrameContexts(frameContextCount, instance, commandBuffers, renderPass);
			}

			auto clearColor = colorCycle.getClearColor(deltaTime);

			auto& acquireNextImageSemaphore = acquireNextImageSemaphores.getAll()[currentResourceIndex];
			auto imageIndex = vkContext.m_swapchain.acquireImage(acquireNextImageSemaphore);

			FrameContext& frameContext = frameContexts[imageIndex];
			frameContext.renderFrame(clearColor, imageIndex, acquireNextImageSemaphore);
		}

		frameId++;

		auto end = std::chrono::steady_clock::now();

		deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - begin).count();
	}

	vkContext.m_device.waitIdle();
}