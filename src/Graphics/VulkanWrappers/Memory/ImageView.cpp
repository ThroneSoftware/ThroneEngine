#include "ImageView.h"

namespace trg
{
	namespace ImageViewPrivate
	{
		auto makeImageView(vk::Device& device,
						   vk::Image& image,
						   vk::ImageAspectFlagBits aspect,
						   vk::ImageViewType viewType,
						   vk::Format format,
						   uint32_t layer,
						   uint32_t layerCount)
		{
			auto componentMapping = vk::ComponentMapping(vk::ComponentSwizzle::eIdentity,
														 vk::ComponentSwizzle::eIdentity,
														 vk::ComponentSwizzle::eIdentity,
														 vk::ComponentSwizzle::eIdentity);

			auto subresourceRange = vk::ImageSubresourceRange(aspect, 0, VK_REMAINING_MIP_LEVELS, layer, layerCount);

			auto imageViewCreateInfo = vk::ImageViewCreateInfo({}, image, viewType, format, componentMapping, subresourceRange);

			return device.createImageViewUnique(imageViewCreateInfo);
		}
	}  // namespace ImageViewPrivate

	ImageView::ImageView(vk::Device& device, vk::ImageView imageView)
	  : m_view(vk::UniqueImageView(imageView, vk::ObjectDestroy(device, nullptr, vk::defaultDispatchLoaderDynamic)))
	{
	}

	ImageView::ImageView(vk::Device& device,
						 vk::Image& image,
						 vk::ImageAspectFlagBits aspect,
						 vk::ImageViewType viewType,
						 vk::Format format,
						 uint32_t layer,
						 uint32_t layerCount)
	  : m_view(ImageViewPrivate::makeImageView(device, image, aspect, viewType, format, layer, layerCount))
	{
	}

	ImageView::VkHandleType& ImageView::getVkHandle()
	{
		return m_view.get();
	}

	const ImageView::VkHandleType& ImageView::getVkHandle() const
	{
		return m_view.get();
	}

	ImageView::VkHandleType& ImageView::operator*()
	{
		return getVkHandle();
	}

	const ImageView::VkHandleType& ImageView::operator*() const
	{
		return getVkHandle();
	}

	ImageView::VkHandleType* ImageView::operator->()
	{
		return &getVkHandle();
	}

	const ImageView::VkHandleType* ImageView::operator->() const
	{
		return &getVkHandle();
	}
}  // namespace trg