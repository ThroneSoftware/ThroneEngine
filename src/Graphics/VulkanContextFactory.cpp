#include "VulkanContextFactory.h"

#include "VulkanContext.h"
#include "VulkanContextFactoryFunctions.h"

#include <Vulkan/vulkan.hpp>
#include <fmt/format.h>

#include <iostream>
#include <mutex>
#include <string>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace trg
{
	extern std::atomic_bool alreadyCreated = false;

	VulkanContextFactory::VulkanContextFactory()
	{
	}

	std::unique_ptr<VulkanContext> VulkanContextFactory::makeContext() const
	{
		if(bool expected = false; alreadyCreated.compare_exchange_strong(expected, true))
		{
			glfwInit();

			vk::DynamicLoader vulkanLibLoader;
			vk::defaultDispatchLoaderDynamic.init(vulkanLibLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr"));

			auto context = std::make_unique<VulkanContext>();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			context->m_swapchainExtent = vk::Extent2D(1024, 768);
			context->m_window =
				glfwCreateWindow(context->m_swapchainExtent.width, context->m_swapchainExtent.height, "Throne", nullptr, nullptr);
			glfwSetWindowUserPointer(context->m_window, context.get());
			glfwSetFramebufferSizeCallback(context->m_window, &glfwFrameBufferResizeCallback);

			auto instance = makeInstance();
			context->m_instance = vk::Instance(instance.instance);
			vk::defaultDispatchLoaderDynamic.init(context->m_instance);

			context->m_messenger = makeDebugMessenger(context->m_instance);

			auto physicalDevice = getPhysicalDevice(instance, *context->m_window);
			context->m_physicalDevice = vk::PhysicalDevice(physicalDevice.physical_device);
			context->m_surface = vk::SurfaceKHR(physicalDevice.surface);

			context->m_vkbDevice = std::make_unique<vkb::Device>(makeDevice(physicalDevice));
			context->m_device = vk::Device(context->m_vkbDevice->device);
			vk::defaultDispatchLoaderDynamic.init(context->m_device);

			vkwrappers::initializeVmaDefaultAllocator(context->m_physicalDevice, context->m_device);

			auto swapchain = makeSwapchain(*context->m_vkbDevice);
			new(&context->m_swapchain) vkwrappers::Swapchain(context->m_device, swapchain);

			context->m_graphicsQueue = makeGraphicsQueue(*context->m_vkbDevice);

			return context;
		}
		else
		{
			// This is because of two things.
			// The vulkan loader since we currently use a default loader.
			// The g_vmaDefaultAllocator.
			throw std::runtime_error("Only one VulkanContext at a time is currently supported.");
		}
	}
}  // namespace trg