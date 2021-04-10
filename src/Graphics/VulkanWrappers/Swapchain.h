#pragma once

#include <Vulkan/vulkan.hpp>


namespace vkb
{
	struct Swapchain;
}

namespace trg
{
	class Swapchain
	{
	public:
		Swapchain() = default;
		Swapchain(vk::Device& device, vkb::Swapchain& swapchain);

		vk::SwapchainKHR& getSwapchain();
		const vk::SwapchainKHR& getSwapchain() const;

	private:
		vk::UniqueSwapchainKHR m_swapchain;

		vk::Format m_imageFormat;
	};
}  // namespace trg