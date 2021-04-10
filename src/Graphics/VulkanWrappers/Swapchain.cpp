#include "Swapchain.h"

#include <VkBootstrap.h>

namespace trg
{
	Swapchain::Swapchain(vk::Device& device, vkb::Swapchain& swapchain)
	  : m_swapchain(vk::UniqueSwapchainKHR(swapchain.swapchain, vk::ObjectDestroy(device, nullptr, vk::defaultDispatchLoaderDynamic)))
	  , m_imageFormat(static_cast<vk::Format>(swapchain.image_format))
	{
	}

	vk::SwapchainKHR& Swapchain::getSwapchain()
	{
		return m_swapchain.get();
	}

	const vk::SwapchainKHR& Swapchain::getSwapchain() const
	{
		return m_swapchain.get();
	}
}  // namespace trg
