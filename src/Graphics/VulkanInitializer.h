#pragma once


// glfw
class GLFWwindow;
// glfw

// vulkan
struct VkQueue;
// vulkan

namespace vkb
{
	class Instance;
	class PhysicalDevice;
	class Device;
	class SwapChain;
}  // namespace vkb

namespace trg
{
	class VulkanInitializer
	{
	public:
		VulkanInitializer();

	private:
		GLFWwindow* m_window;


	};
}  // namespace trg