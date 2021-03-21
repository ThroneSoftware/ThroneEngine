#pragma once

#include "VulkanWrappers/Swapchain.h"

#include <VkBootstrap.h>
#include <Vulkan/vulkan.hpp>

struct GLFWwindow;

namespace trg
{
	class VulkanContext
	{
	public:
		VulkanContext();
		~VulkanContext();

		GLFWwindow* m_window;

		vk::Instance m_instance;
		vk::DebugUtilsMessengerEXT m_messenger;

		vk::PhysicalDevice m_physicalDevice;
		vk::Device m_device;
		union
		{
			Swapchain m_swapchain;
		};
		vk::SurfaceKHR m_surface;

		vk::Queue m_graphicsQueue;
		vk::Queue m_presentQueue;

		std::vector<vk::Semaphore> m_presentSemaphores;
		std::vector<vk::SwapchainKHR> m_swapchains;
	};
}  // namespace trg