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
		using VkHandleType = vk::SwapchainKHR;

	public:
		Swapchain() = default;
		Swapchain(vk::Device& device, vkb::Swapchain& swapchain);

		VkHandleType& getVkHandle();
		const VkHandleType& getVkHandle() const;

		VkHandleType& operator*();
		const VkHandleType& operator*() const;

		VkHandleType* operator->();
		const VkHandleType* operator->() const;

		std::span<ImageView> getImageViews();
		std::span<const ImageView> getImageViews() const;

		vk::Format getFormat() const;

	private:
		vk::UniqueSwapchainKHR m_swapchain;
		std::vector<ImageView> m_imageViews;

		vk::Format m_imageFormat;
	};
}  // namespace trg