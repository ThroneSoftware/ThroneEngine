#include "VulkanContextFactory.h"

#include "VulkanContext.h"
#include "VulkanWrappers/Memory/Buffer.h"
#include "VulkanWrappers/Memory/VmaAllocator.h"

#include <GLFW/glfw3.h>

#include <VkBootstrap.h>
#include <Vulkan/vulkan.hpp>
#include <fmt/format.h>

#include <iostream>
#include <mutex>
#include <string>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace trg
{
	namespace VulkanContextFactoryPrivate
	{
		const char* toString(VkDebugUtilsMessageTypeFlagsEXT messageType)
		{
			switch(messageType)
			{
				case(7):
					return "General | Validation | Performance";
				case(6):
					return "Validation | Performance";
				case(5):
					return "General | Performance";
				case(4):
					return "Performance";
				case(3):
					return "General | Validation";
				case(2):
					return "Validation";
				case(1):
					return "General";
				default:
					return "Unknown";
			}
		}

		vk::Bool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
								 VkDebugUtilsMessageTypeFlagsEXT messageType,
								 const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
								 void*)
		{
			auto messageSeverityString = vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity));

			auto message = fmt::format("[{0} : {1}]\n{2}\n", messageSeverityString, toString(messageType), pCallbackData->pMessage);

			std::cout << message << std::endl;

			return false;
		}

		vkb::Instance makeInstance()
		{
			auto instanceResult =
				vkb::InstanceBuilder().enable_extension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME).request_validation_layers().build();

			if(!instanceResult.has_value())
			{
				throw std::runtime_error(fmt::format("Instance initialization failed. Error code: {0}. Error message: {1}",
													 instanceResult.error().value(),
													 instanceResult.error().message()));
			}

			return instanceResult.value();
		}

		auto makeDebugMessenger(vk::Instance& instance)
		{
			auto messageTypes = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
								vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
			auto severity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;

			auto debugMessengerCreateInfo =
				vk::DebugUtilsMessengerCreateInfoEXT(vk::DebugUtilsMessengerCreateFlagBitsEXT(0), severity, messageTypes, &debugCallback);

			return instance.createDebugUtilsMessengerEXT(debugMessengerCreateInfo);
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

		vkb::Swapchain makeSwapchain(const vkb::Device& device)
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
	}  // namespace VulkanContextFactoryPrivate


	VulkanContextFactory::VulkanContextFactory()
	{
	}

	std::unique_ptr<VulkanContext> VulkanContextFactory::makeContext() const
	{
		std::atomic_bool alreadyCreated = false;

		if(bool expected = false; alreadyCreated.compare_exchange_strong(expected, true))
		{
			glfwInit();

			vk::DynamicLoader vulkanLibLoader;
			vk::defaultDispatchLoaderDynamic.init(vulkanLibLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr"));

			auto context = std::make_unique<VulkanContext>();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			context->m_window = glfwCreateWindow(1024, 768, "Throne", nullptr, nullptr);

			using namespace VulkanContextFactoryPrivate;


			auto instance = makeInstance();
			context->m_instance = vk::Instance(instance.instance);
			vk::defaultDispatchLoaderDynamic.init(context->m_instance);

			context->m_messenger = makeDebugMessenger(context->m_instance);

			auto physicalDevice = getPhysicalDevice(instance, *context->m_window);
			context->m_physicalDevice = vk::PhysicalDevice(physicalDevice.physical_device);
			context->m_surface = vk::SurfaceKHR(physicalDevice.surface);

			auto device = makeDevice(physicalDevice);
			context->m_device = vk::Device(device.device);
			vk::defaultDispatchLoaderDynamic.init(context->m_device);

			initializeVmaDefaultAllocator(context->m_physicalDevice, context->m_device);

			auto swapchain = makeSwapchain(device);
			new(&context->m_swapchain) Swapchain(context->m_device, swapchain);
			context->m_swapchains = {context->m_swapchain.getSwapchain()};

			context->m_presentQueue = vk::Queue(makePresentQueue(device));

			context->m_graphicsQueue = vk::Queue(makeGraphicsQueue(device));

			return context;
		}
		else
		{
			// This is because of two things.
			// The vulkan loader since we currently use a default loader.
			// The vmaDefaultAllocator.
			throw std::runtime_error("Only one VulkanContext at a time is currently supported.");
		}
	}
}  // namespace trg