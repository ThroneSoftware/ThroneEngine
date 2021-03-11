#include "VulkanInitializer.h"

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

		vkb::PhysicalDevice getPhysicalDevice(const vkb::Instance& instance)
		{
			// needs to add the surface
			auto physicalDeviceResult = vkb::PhysicalDeviceSelector(instance).select();

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
				throw std::runtime_error(fmt::format("Graphics queue initialization failed. Error code: {0}. Error message: {1}",
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
		// needs to create the window
		auto instance = VulkanInitializerPrivate::makeInstance();
		auto physicalDevice = VulkanInitializerPrivate::getPhysicalDevice(instance);
		auto device = VulkanInitializerPrivate::makeDevice(physicalDevice);
		auto swapChain = VulkanInitializerPrivate::makeSwapChain(device);
		auto graphicsQueue = VulkanInitializerPrivate::makeGraphicsQueue(device);
		auto presentQueue = VulkanInitializerPrivate::makePresentQueue(device);
	}
}  // namespace trg