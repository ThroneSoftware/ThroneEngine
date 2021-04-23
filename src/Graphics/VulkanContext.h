#pragma once

#include "VulkanWrappers/Commands/CommandQueue.h"
#include "VulkanWrappers/Swapchain.h"
#include "VulkanWrappers/Syncronization/SemaphorePool.h"

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
		vk::Extent2D m_swapchainExtent;

		vk::Instance m_instance;
		vk::DebugUtilsMessengerEXT m_messenger;

		vk::PhysicalDevice m_physicalDevice;
		vk::Device m_device;
		union
		{
			Swapchain m_swapchain;
		};
		vk::SurfaceKHR m_surface;

		CommandQueue m_graphicsQueue;

		std::vector<vk::SwapchainKHR> m_swapchains;
	};
}  // namespace trg