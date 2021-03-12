#include "VulkanInitializer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <VkBootstrap.h>
#include <Vulkan/vulkan.hpp>
#include <fmt/format.h>

namespace trg
{
	namespace VulkanInitializerPrivate
	{
		vkb::Instance makeInstance()
		{
			auto instanceResult = vkb::InstanceBuilder().use_default_debug_messenger().request_validation_layers().build();

			if(!instanceResult.has_value())
			{
				throw std::runtime_error(fmt::format("Instance initialization failed. Error code: {0}. Error message: {1}",
													 instanceResult.error().value(),
													 instanceResult.error().message()));
			}

			return instanceResult.value();
		}

		vkb::PhysicalDevice getPhysicalDevice(const vkb::Instance& instance, GLFWwindow& window)
		{
			VkSurfaceKHR surface;
			VkResult result = glfwCreateWindowSurface(instance.instance, &window, nullptr, &surface);

			auto physicalDeviceResult = vkb::PhysicalDeviceSelector(instance).set_surface(surface).select();

			if(!physicalDeviceResult.has_value())
			{
				throw std::runtime_error(fmt::format("Physical device initialization failed. Error code: {0}. Error message: {1}",
													 physicalDeviceResult.error().value(),
													 physicalDeviceResult.error().message()));
			}

			return physicalDeviceResult.value();
		}

		vkb::Device makeDevice(const vkb::PhysicalDevice& physicalDevice)
		{
			auto deviceResult = vkb::DeviceBuilder(physicalDevice).build();

			if(!deviceResult.has_value())
			{
				throw std::runtime_error(fmt::format("Device initialization failed. Error code: {0}. Error message: {1}",
													 deviceResult.error().value(),
													 deviceResult.error().message()));
			}

			return deviceResult.value();
		}

		vkb::Swapchain makeSwapChain(const vkb::Device& device)
		{
			auto swapChainResult = vkb::SwapchainBuilder(device).build();

			if(!swapChainResult.has_value())
			{
				throw std::runtime_error(fmt::format("Swapchain initialization failed. Error code: {0}. Error message: {1}",
													 swapChainResult.error().value(),
													 swapChainResult.error().message()));
			}

			return swapChainResult.value();
		}

		VkQueue makeGraphicsQueue(const vkb::Device& device)
		{
			auto queueResult = device.get_queue(vkb::QueueType::graphics);

			if(!queueResult.has_value())
			{
				throw std::runtime_error(fmt::format("Graphics queue initialization failed. Error code: {0}. Error message: {1}",
													 queueResult.error().value(),
													 queueResult.error().message()));
			}

			return queueResult.value();
		}

		VkQueue makePresentQueue(const vkb::Device& device)
		{
			auto queueResult = device.get_queue(vkb::QueueType::present);

			if(!queueResult.has_value())
			{
				throw std::runtime_error(fmt::format("Present queue initialization failed. Error code: {0}. Error message: {1}",
													 queueResult.error().value(),
													 queueResult.error().message()));
			}

			return queueResult.value();
		}
	}  // namespace VulkanInitializerPrivate


	VulkanInitializer::VulkanInitializer()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_window = glfwCreateWindow(1024, 768, "Throne", nullptr, nullptr);

		using namespace VulkanInitializerPrivate;

		m_instance = std::make_unique<vkb::Instance>(makeInstance());
		m_physicalDevice = std::make_unique<vkb::PhysicalDevice>(getPhysicalDevice(*m_instance, *m_window));
		m_device = std::make_unique<vkb::Device>(makeDevice(*m_physicalDevice));
		m_swapchain = std::make_unique<vkb::Swapchain>(makeSwapChain(*m_device));
		m_graphicsQueue = std::make_unique<VkQueue>(makeGraphicsQueue(*m_device));
		m_presentQueue = std::make_unique<VkQueue>(makePresentQueue(*m_device));
	}

	VulkanInitializer::~VulkanInitializer()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}  // namespace trg