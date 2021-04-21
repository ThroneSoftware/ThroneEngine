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
		m_presentWaitSemaphores.~SemaphorePool();

		m_instance.destroySurfaceKHR(m_surface);
		m_instance.destroyDebugUtilsMessengerEXT(m_messenger);

		g_vmaDefaultAllocator.destroy();

		m_device.destroy();
		m_instance.destroy();

		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}  // namespace trg