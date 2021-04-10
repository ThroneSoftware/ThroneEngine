#include "Image.h"

#include "VmaAllocator.h"

namespace trg
{
	namespace ImagePrivate
	{
		auto makeImage()
		{
			vk::ImageCreateInfo imageInfo = vk::ImageCreateInfo();

			vma::AllocationCreateInfo allocationCreateInfo = vma::AllocationCreateInfo();
			allocationCreateInfo.usage = vma::MemoryUsage::eGpuOnly;

			return g_vmaDefaultAllocator.createImage(imageInfo, allocationCreateInfo);
		}
	}  // namespace ImagePrivate

	Image::Image(vk::Device& device)
	  : m_device(device)
	  , m_image(ImagePrivate::makeImage())
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
}  // namespace trg