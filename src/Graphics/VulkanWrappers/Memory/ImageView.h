#pragma once

#include "Vulkan/vulkan.hpp"

namespace trg
{
	class ImageView
	{
	public:
		using VkHandleType = vk::ImageView;

	public:
		ImageView(vk::Device& device, vk::ImageView&& imageView);
		ImageView(vk::Device& device,
				  vk::Image& image,
				  vk::ImageAspectFlagBits aspect,
				  vk::ImageViewType viewType,
				  vk::Format format,
				  uint32_t layer,
				  uint32_t layerCount);

		VkHandleType& getVkHandle();
		const VkHandleType& getVkHandle() const;

		VkHandleType& operator*();
		const VkHandleType& operator*() const;

		VkHandleType* operator->();
		const VkHandleType* operator->() const;

	private:
		vk::UniqueImageView m_view;
	};
}  // namespace trg