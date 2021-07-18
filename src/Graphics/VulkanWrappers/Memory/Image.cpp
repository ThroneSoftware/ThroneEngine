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
					   vk::ImageUsageFlags usage,
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
	  , m_dimensions(dimensions)
	  , m_image(ImagePrivate::makeImage(type,
										format,
										dimensions,
										mipmapCount,
										layerCount,
										samples,
										imageTiling,
										usage | vk::ImageUsageFlagBits::eTransferDst,
										layout,
										memoryUsage))
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
		auto stagingBuffer = Buffer(memory.byteSize, vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst, vma::MemoryUsage::eCpuToGpu);

		stagingBuffer.updateWithHostMemory(memory);

		auto command = [this, aspectToUpdate, newAccess, newLayout, newPipelineStage, &stagingBuffer](CommandBuffer& commandBuffer) {
			auto subresourceRange = vk::ImageSubresourceRange(aspectToUpdate,
															  0 /*baseMipLevel*/,
															  VK_REMAINING_MIP_LEVELS,
															  0 /*baseArrayLayer*/,
															  VK_REMAINING_ARRAY_LAYERS);

			auto imageTransition = vk::ImageMemoryBarrier(m_accessFlags,
														  vk::AccessFlagBits::eTransferWrite,
														  m_imageLayout,
														  vk::ImageLayout::eTransferDstOptimal,
														  VK_QUEUE_FAMILY_IGNORED,
														  VK_QUEUE_FAMILY_IGNORED,
														  m_image.m_value,
														  subresourceRange);

			commandBuffer->pipelineBarrier(m_pipelineStage,
										   vk::PipelineStageFlagBits::eTransfer,
										   {} /*dependencyFlags*/,
										   {} /*memoryBarriers*/,
										   {} /*bufferMemoryBarriers*/,
										   imageTransition);


			auto imageSubresourcesLayer =
				vk::ImageSubresourceLayers(aspectToUpdate, 0 /*mipLevel*/, 0 /*baseArrayLayer*/, 1 /*layerCount*/);
			auto bufferRegion = vk::BufferImageCopy(0 /*bufferOffset*/,
													0 /*bufferRowLength*/,
													0 /*bufferImageHeight*/,
													imageSubresourcesLayer,
													{0, 0, 0} /*imageOffset*/,
													m_dimensions);

			commandBuffer->copyBufferToImage(*stagingBuffer, m_image.m_value, vk::ImageLayout::eTransferDstOptimal, bufferRegion);

			imageTransition.srcAccessMask = imageTransition.dstAccessMask;
			imageTransition.dstAccessMask = newAccess;
			imageTransition.oldLayout = imageTransition.newLayout;
			imageTransition.newLayout = newLayout;

			commandBuffer->pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
										   newPipelineStage,
										   {} /*dependencyFlags*/,
										   {} /*memoryBarriers*/,
										   {} /*bufferMemoryBarriers*/,
										   imageTransition);
		};

		commandQueue.immediateSubmit(command);

		m_imageLayout = newLayout;
		m_accessFlags = newAccess;
		m_pipelineStage = newPipelineStage;
	}
}  // namespace trg::vkwrappers