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

	Image::Image()
	  : m_image(ImagePrivate::makeImage())
	{
	}
}  // namespace trg