#pragma once

#include "ImageView.h"
#include "VmaUnique.h"

#include <Vendors/VulkanMemoryAllocator/vk_mem_alloc.hpp>
#include <Vulkan/vulkan.hpp>

#include <span>

namespace trg
{
	class Image
	{
	public:
		Image(vk::Device& device);

		void addImageView(vk::ImageAspectFlagBits aspect,
						  vk::ImageViewType viewType,
						  vk::Format format,
						  uint32_t layer,
						  uint32_t layerCount);

		ImageView& getImageView();
		const ImageView& getImageView() const;

		std::span<ImageView> getImageViews();
		std::span<const ImageView> getImageViews() const;

	private:
		vk::Device& m_device;

		VmaUnique<vk::Image> m_image;
		std::vector<ImageView> m_imageViews;
	};
}  // namespace trg