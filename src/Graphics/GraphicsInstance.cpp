#include "GraphicsInstance.h"

#include "Utility.h"
#include "Vulkan/vulkan.hpp"
#include "VulkanContext.h"

namespace trg
{
	GraphicsInstance::GraphicsInstance(std::unique_ptr<GraphicsContext> context)
	  : m_context(std::move(context))
	{
	}

	void GraphicsInstance::present()
	{
		VulkanContext& vkContext = vulkanContext();


		auto index = vkContext.m_device.acquireNextImageKHR(vkContext.m_swapchain.getSwapchain(),
															std::numeric_limits<std::uint64_t>::max(),
															vk::Semaphore(),
															vk::Fence());


		auto indices = {index.value};

		auto vkPresentWaitSemaphores = toVkHandle(vkContext.m_presentWaitSemaphores.getAll());
		auto presentInfo = vk::PresentInfoKHR(vkPresentWaitSemaphores, vkContext.m_swapchains, indices);

		auto result = vkContext.m_graphicsQueue->presentKHR(presentInfo);
	}

	VulkanContext& GraphicsInstance::vulkanContext()
	{
		return *m_context->m_vulkanContext;
	}
}  // namespace trg