#pragma once

#include <memory>

// glfw
struct GLFWwindow;
// glfw

// vulkan
typedef VkQueue_T* VkQueue;
// vulkan

namespace vkb
{
	struct Instance;
	struct PhysicalDevice;
	struct Device;
	struct Swapchain;
}  // namespace vkb

namespace trg
{
	class VulkanInitializer
	{
	public:
		VulkanInitializer();

		~VulkanInitializer();

	private:
		GLFWwindow* m_window;

		std::unique_ptr<vkb::Instance> m_instance;
		std::unique_ptr<vkb::PhysicalDevice> m_physicalDevice;
		std::unique_ptr<vkb::Device> m_device;
		std::unique_ptr<vkb::Swapchain> m_swapchain;

		std::unique_ptr<VkQueue> m_graphicsQueue;
		std::unique_ptr<VkQueue> m_presentQueue;
	};
}  // namespace trg