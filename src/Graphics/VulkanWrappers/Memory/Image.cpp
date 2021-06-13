#include "Image.h"

#include "..\..\Images\Image.h"
#include "Buffer.h"
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

	void Image::updateWithHostMemory(tru::MemoryRegion memory)
	{
		allocateHostMemory(memory, m_image.m_allocation);
	}

	void Image::updateWithDeviceLocalMemory(CommandQueue& commandQueue,
											tru::MemoryRegion memory,
											vk::ImageAspectFlags aspectToUpdate,
											vk::ImageLayout newLayout,
											vk::AccessFlagBits newAccess,
											vk::PipelineStageFlagBits newPipelineStage)
	{
		auto stagingBuffer = Buffer(memory.byteSize, vk::BufferUsageFlagBits::eTransferSrc, vma::MemoryUsage::eCpuToGpu);

		stagingBuffer.updateWithHostMemory(memory);

		auto command = [](CommandBuffer& commandBuffer) {
		};

		commandQueue.immediateSubmit(command);
	}
}  // namespace trg::vkwrappers