#pragma once

#include "VulkanContext.h"
#include "VulkanWrappers/Commands/CommandQueue.h"
#include "VulkanWrappers/Memory/Buffer.h"
#include "VulkanWrappers/Memory/VmaAllocator.h"

#include <GLFW/glfw3.h>

#include <VkBootstrap.h>
#include <Vulkan/vulkan.hpp>
#include <fmt/format.h>

#include <iostream>
#include <string>

namespace trg
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
		auto instanceResult = vkb::InstanceBuilder()
								  .require_api_version(1, 2)
								  .enable_extension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
								  .request_validation_layers()
								  .build();

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
		auto features = vk::PhysicalDeviceVulkan12Features();
		features.separateDepthStencilLayouts = true;

		auto deviceResult = vkb::DeviceBuilder(physicalDevice).add_pNext(&features).build();

		if(!deviceResult.has_value())
		{
			throw std::runtime_error(fmt::format("Device initialization failed. Error code: {0}. Error message: {1}",
												 deviceResult.error().value(),
												 deviceResult.error().message()));
		}

		return deviceResult.value();
	}

	vkb::Swapchain makeSwapchain(const vkb::Device& device, VkSwapchainKHR oldSwapchain = VK_NULL_HANDLE)
	{
		auto swapChainResult = vkb::SwapchainBuilder(device).set_old_swapchain(oldSwapchain).build();

		if(!swapChainResult.has_value())
		{
			throw std::runtime_error(fmt::format("Swapchain initialization failed. Error code: {0}. Error message: {1}",
												 swapChainResult.error().value(),
												 swapChainResult.error().message()));
		}

		return swapChainResult.value();
	}

	CommandQueue makeGraphicsQueue(const vkb::Device& device)
	{
		auto queueResult = device.get_queue(vkb::QueueType::graphics);

		if(!queueResult.has_value())
		{
			throw std::runtime_error(fmt::format("Graphics queue initialization failed. Error code: {0}. Error message: {1}",
												 queueResult.error().value(),
												 queueResult.error().message()));
		}

		return CommandQueue(vk::Queue(queueResult.value()), device.get_queue_index(vkb::QueueType::graphics).value());
	}

	void glfwFrameBufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		void* userPointer = glfwGetWindowUserPointer(window);
		VulkanContext* vkContext = reinterpret_cast<VulkanContext*>(userPointer);

		vkContext->hasWindowResizeEvent = true;

		vkContext->m_device.waitIdle();

		vkContext->m_swapchainExtent = vk::Extent2D(width, height);

		auto swapchain = makeSwapchain(*vkContext->m_vkbDevice, vkContext->m_swapchain.getSwapchain());
		vkContext->m_swapchain.~Swapchain();
		new(&vkContext->m_swapchain) Swapchain(vkContext->m_device, swapchain);
	}
}  // namespace trg