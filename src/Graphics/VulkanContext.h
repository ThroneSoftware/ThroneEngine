#pragma once

#include "VulkanWrappers/Commands/CommandQueue.h"
#include "VulkanWrappers/Swapchain.h"
#include "VulkanWrappers/Syncronization/SemaphorePool.h"

#include <Vulkan/vulkan.hpp>

struct GLFWwindow;

namespace vkb
{
	struct Device;
}

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

		std::unique_ptr<vkb::Device> m_vkbDevice;

		vk::SurfaceKHR m_surface;

		union
		{
			vkwrappers::Swapchain m_swapchain;
		};

		union
		{
			vkwrappers::CommandQueue m_graphicsQueue;
		};

		std::atomic_bool m_hasWindowResizeEvent = false;
		std::atomic_bool m_windowMinimized = false;

		glm::ivec2 m_mousePosition;
		std::optional<glm::ivec2> m_mouseMove;
	};
}  // namespace trg