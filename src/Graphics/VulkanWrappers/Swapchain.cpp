#include "Swapchain.h"

#include <VkBootstrap.h>

namespace trg
{
	Swapchain::Swapchain(vk::Device& device, vkb::Swapchain& swapchain)
	  : m_device(device)
	  , m_swapchain(swapchain.swapchain)
	  , m_imageFormat(swapchain.image_format)
	{
	}

	Swapchain::~Swapchain()
	{
		reset();
	}

	void Swapchain::reset()
	{
		m_device.destroySwapchainKHR(m_swapchain);
		m_swapchain = nullptr;
	}

	vk::SwapchainKHR& Swapchain::getSwapchain()
	{
		return m_swapchain;
	}

	const vk::SwapchainKHR& Swapchain::getSwapchain() const
	{
		return m_swapchain;
	}
}  // namespace trg
