#include "Image.h"

#include "..\..\Images\Image.h"
#include "VmaAllocator.h"

namespace trg::vkwrappers
{
	namespace ImagePrivate
	{
		auto makeImage(vk::ImageType type,
					   vk::Format format,
					   vk::Extent3D dimensions,
					   uint32_t mipmapCount,
					   uint32_t layerCount,
					   vk::SampleCountFlagBits samples,
					   vk::ImageTiling imageTiling,
					   vk::ImageUsageFlagBits usage,
					   vk::ImageLayout layout,
					   vma::MemoryUsage memoryUsage)
		{
			auto imageInfo = vk::ImageCreateInfo({},
												 type,
												 format,
												 dimensions,
												 mipmapCount,
												 layerCount,
												 samples,
												 imageTiling,
												 usage,
												 vk::SharingMode::eExclusive,
												 {},
												 layout);

			vma::AllocationCreateInfo allocationCreateInfo = vma::AllocationCreateInfo();
			allocationCreateInfo.usage = memoryUsage;

			return g_vmaDefaultAllocator.createImage(imageInfo, allocationCreateInfo);
		}
	}  // namespace ImagePrivate

	Image::Image(vk::Device& device,
				 vk::ImageType type,
				 vk::Format format,
				 vk::Extent3D dimensions,
				 uint32_t mipmapCount,
				 uint32_t layerCount,
				 vk::SampleCountFlagBits samples,
				 vk::ImageTiling imageTiling,
				 vk::ImageUsageFlagBits usage,
				 vk::ImageLayout layout,
				 vma::MemoryUsage memoryUsage)
	  : m_device(device)
	  , m_image(
			ImagePrivate::makeImage(type, format, dimensions, mipmapCount, layerCount, samples, imageTiling, usage, layout, memoryUsage))
	  , m_imageLayout(layout)
	{
	}

	void Image::addImageView(vk::ImageAspectFlagBits aspect,
							 vk::ImageViewType viewType,
							 vk::Format format,
							 uint32_t layer,
							 uint32_t layerCount)
	{
		m_imageViews.emplace_back(m_device, m_image.m_value, aspect, viewType, format, layer, layerCount);
	}

	ImageView& Image::getImageView()
	{
		assert(!m_imageViews.empty());
		return m_imageViews[0];
	}

	const ImageView& Image::getImageView() const
	{
		assert(!m_imageViews.empty());
		return m_imageViews[0];
	}

	std::span<ImageView> Image::getImageViews()
	{
		return m_imageViews;
	}

	std::span<const ImageView> Image::getImageViews() const
	{
		return m_imageViews;
	}

	vk::ImageLayout Image::getImageLayout() const
	{
		return m_imageLayout;
	}

	void Image::updateWithHostMemory(vk::DeviceSize dataSize, const void* srcData)
	{
		allocateHostMemory(dataSize, srcData, m_image.m_allocation);
	}
}  // namespace trg::vkwrappers