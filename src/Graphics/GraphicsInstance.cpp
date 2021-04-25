#include "GraphicsInstance.h"

#include "Utility.h"
#include "Vulkan/vulkan.hpp"
#include "VulkanContext.h"
#include "VulkanContextFactoryFunctions.h"

#include <GLFW/glfw3.h>

namespace trg
{
	GraphicsInstance::GraphicsInstance(std::unique_ptr<GraphicsContext> context)
	  : m_context(std::move(context))
	{
	}

	void GraphicsInstance::processGLFWEvents()
	{
		glfwPollEvents();
	}

	void GraphicsInstance::present(uint32_t imageIndex, std::vector<vk::Semaphore>& waitSemaphores)
	{
		auto indices = {imageIndex};

		auto presentInfo = vk::PresentInfoKHR(waitSemaphores, vulkanContext().m_swapchain.getSwapchain(), indices);

		auto result = vulkanContext().m_graphicsQueue->presentKHR(presentInfo);
	}

	VulkanContext& GraphicsInstance::vulkanContext()
	{
		return *m_context->m_vulkanContext;
	}

	bool GraphicsInstance::windowShouldClose()
	{
		return glfwWindowShouldClose(m_context->m_vulkanContext->m_window);
	}
}  // namespace trg