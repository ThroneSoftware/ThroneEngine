#pragma once

#include "ImageView.h"
#include "VmaUnique.h"

#include <Vendors/VulkanMemoryAllocator/vk_mem_alloc.hpp>
#include <Vulkan/vulkan.hpp>

#include <span>

namespace trg::vkwrappers
{
	class Image
	{
	public:
		Image(vk::Device& device,
			  vk::ImageType type,
			  vk::Format format,
			  vk::Extent3D dimensions,
			  uint32_t mipmapCount,
			  uint32_t layerCount,
			  vk::SampleCountFlagBits samples,
			  vk::ImageUsageFlagBits usage,
			  vk::ImageLayout layout);

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
		std::reference_wrapper<vk::Device> m_device;

		VmaUnique<vk::Image> m_image;
		std::vector<ImageView> m_imageViews;
	};
}  // namespace trg::vkwrappers