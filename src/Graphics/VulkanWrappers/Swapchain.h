#pragma once

#include "Memory/ImageView.h"

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

		vk::Format getFormat() const;

		std::span<ImageView> getImageViews();
		std::span<const ImageView> getImageViews() const;

	private:
		vk::UniqueSwapchainKHR m_swapchain;
		std::vector<ImageView> m_imageViews;

		vk::Format m_imageFormat;
	};
}  // namespace trg