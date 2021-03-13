#include "VulkanContext.h"

namespace trg
{
	VulkanContext::VulkanContext(GLFWwindow* window,
								 vkb::Instance&& instance,
								 vkb::PhysicalDevice&& physicalDevice,
								 vkb::Device&& device,
								 vkb::Swapchain&& swapchain,
								 VkQueue graphicsQueue,
								 VkQueue presentQueue)
	  : m_window(window)
	  , m_instance(std::move(instance))
	  , m_physicalDevice(std::move(physicalDevice))
	  , m_device(std::move(device))
	  , m_swapchain(std::move(swapchain))
	  , m_graphicsQueue(graphicsQueue)
	  , m_presentQueue(presentQueue)
	{
	}


	VulkanContext::~VulkanContext()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}  // namespace trg