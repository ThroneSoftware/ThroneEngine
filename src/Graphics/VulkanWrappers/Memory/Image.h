#pragma once

#include "../../Images/Image.h"
#include "../Commands/CommandQueue.h"
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
			  vk::ImageTiling imageTiling,
			  vk::ImageUsageFlagBits usage,
			  vk::ImageLayout layout,
			  vma::MemoryUsage memoryUsage);

		void addImageView(vk::ImageAspectFlagBits aspect,
						  vk::ImageViewType viewType,
						  vk::Format format,
						  uint32_t layer,
						  uint32_t layerCount);

		ImageView& getImageView();
		const ImageView& getImageView() const;

		std::span<ImageView> getImageViews();
		std::span<const ImageView> getImageViews() const;

		vk::ImageLayout getImageLayout() const;

		void updateWithHostMemory(tru::MemoryRegion memory);
		void updateWithDeviceLocalMemory(CommandQueue& commandQueue,
										 tru::MemoryRegion memory,
										 vk::ImageAspectFlags aspectToUpdate,
										 vk::ImageLayout newLayout,
										 vk::AccessFlagBits newAccess,
										 vk::PipelineStageFlagBits newPipelineStage);

	private:
		std::reference_wrapper<vk::Device> m_device;

		vk::Extent3D m_dimensions;

		VmaUnique<vk::Image> m_image;
		std::vector<ImageView> m_imageViews;

		vk::ImageLayout m_imageLayout;
		vk::AccessFlagBits m_accessFlags = vk::AccessFlagBits::eNoneKHR;
		vk::PipelineStageFlagBits m_pipelineStage = vk::PipelineStageFlagBits::eTopOfPipe;
	};

	inline vk::Format imageLayoutToVkFormat(ImageLayout layout)
	{
		switch(layout)
		{
			case trg::ImageLayout::RgbaU8:
				return vk::Format::eR8G8B8A8Unorm;
			case trg::ImageLayout::RgbU8:
				return vk::Format::eR8G8B8Unorm;
			default:
				assert(false);
				throw std::runtime_error("Invalid Range.");
				break;
		}
	}
}  // namespace trg::vkwrappers