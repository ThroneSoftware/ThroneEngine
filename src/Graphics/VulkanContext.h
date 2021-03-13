#pragma once

#include <GLFW/glfw3.h>

#include <VkBootstrap.h>
#include <Vulkan/vulkan.hpp>

namespace trg
{
	class VulkanContext
	{
	public:
		VulkanContext(GLFWwindow* window,
					  vkb::Instance&& instance,
					  vkb::PhysicalDevice&& physicalDevice,
					  vkb::Device&& device,
					  vkb::Swapchain&& swapchain,
					  VkQueue graphicsQueue,
					  VkQueue presentQueue);

		~VulkanContext();

		GLFWwindow* m_window;

		vkb::Instance m_instance;
		vkb::PhysicalDevice m_physicalDevice;
		vkb::Device m_device;
		vkb::Swapchain m_swapchain;

		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;
	};
}  // namespace trg