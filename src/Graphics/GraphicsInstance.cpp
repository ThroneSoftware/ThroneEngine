#include "GraphicsInstance.h"

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

		vk::DynamicLoader libLoader;
		vk::DispatchLoaderDynamic dispatch;
		dispatch.init(vk::Instance(vkContext.m_instance.instance), vk::Device(vkContext.m_device.device), libLoader);

		auto semaphores = std::initializer_list<VkSemaphore>{};
		auto swapchains = std::initializer_list<VkSwapchainKHR>{vkContext.m_swapchain.swapchain};
		uint32_t indice = 0;

		dispatch.vkAcquireNextImageKHR(vkContext.m_device.device,
									   vkContext.m_swapchain.swapchain,
									   std::numeric_limits<std::uint64_t>::max(),
									   VK_NULL_HANDLE,
									   VK_NULL_HANDLE,
									   &indice);

		VkPresentInfoKHR presentInfo = {};
		presentInfo.pNext = nullptr;
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pWaitSemaphores = semaphores.begin();
		presentInfo.waitSemaphoreCount = static_cast<uint32_t>(semaphores.size());

		presentInfo.pSwapchains = swapchains.begin();
		presentInfo.swapchainCount = static_cast<uint32_t>(swapchains.size());

		presentInfo.pImageIndices = &indice;


		dispatch.vkQueuePresentKHR(vkContext.m_presentQueue, &presentInfo);
	}

	VulkanContext& GraphicsInstance::vulkanContext()
	{
		return *m_context->m_vulkanContext;
	}
}  // namespace trg