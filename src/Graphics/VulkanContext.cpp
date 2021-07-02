#include "VulkanContext.h"

#include "VulkanWrappers/Memory/VmaAllocator.h"

#include <GLFW/glfw3.h>

namespace trg
{
	VulkanContext::VulkanContext()
	{
	}

	VulkanContext::~VulkanContext()
	{
		m_swapchain.~Swapchain();

		m_graphicsQueue.~CommandQueue();

		m_instance.destroySurfaceKHR(m_surface);

		vkwrappers::g_vmaDefaultAllocator.destroy();

		m_device.destroy();

		m_instance.destroyDebugUtilsMessengerEXT(m_messenger);

		m_instance.destroy();

		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}  // namespace trg