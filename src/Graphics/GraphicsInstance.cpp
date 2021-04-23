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

	void GraphicsInstance::present(uint32_t imageIndex, std::vector<vk::Semaphore>& waitSemaphores)
	{
		auto indices = { imageIndex };

		auto presentInfo = vk::PresentInfoKHR(waitSemaphores, vulkanContext().m_swapchains, indices);

		auto result = vulkanContext().m_graphicsQueue->presentKHR(presentInfo);
	}

	VulkanContext& GraphicsInstance::vulkanContext()
	{
		return *m_context->m_vulkanContext;
	}
}  // namespace trg