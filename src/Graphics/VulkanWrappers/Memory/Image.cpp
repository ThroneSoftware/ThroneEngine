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

			return vmaDefaultAllocator.createImage(imageInfo, allocationCreateInfo);
		}
	}  // namespace ImagePrivate

	Image::Image()
	  : Image(ImagePrivate::makeImage())
	{
	}

	Image::~Image()
	{
		vmaDefaultAllocator.destroyImage(m_image, m_allocation);
	}

	Image::Image(std::pair<vk::Image, vma::Allocation>&& image)
	  : m_image(std::move(image.first))
	  , m_allocation(image.second)
	{
	}
}  // namespace trg