#include "Swapchain.h"

#include <VkBootstrap.h>

namespace trg
{
	namespace SwapchainPrivate
	{
		auto getImageViews(vk::Device& device, vkb::Swapchain& swapchain)
		{
			auto vkImageViews = swapchain.get_image_views().value();
			std::vector<ImageView> imageViews;
			imageViews.reserve(vkImageViews.size());
			std::transform(vkImageViews.begin(), vkImageViews.end(), std::back_inserter(imageViews), [&device](VkImageView imageView) {
				return ImageView(device, vk::ImageView(imageView));
			});
			return imageViews;
		}
	}  // namespace SwapchainPrivate

	Swapchain::Swapchain(vk::Device& device, vkb::Swapchain& swapchain)
	  : m_swapchain(vk::UniqueSwapchainKHR(swapchain.swapchain, vk::ObjectDestroy(device, nullptr, vk::defaultDispatchLoaderDynamic)))
	  , m_imageViews(SwapchainPrivate::getImageViews(device, swapchain))
	  , m_imageFormat(static_cast<vk::Format>(swapchain.image_format))
	{
	}

	vk::SwapchainKHR& Swapchain::getSwapchain()
	{
		return m_swapchain.get();
	}

	const vk::SwapchainKHR& Swapchain::getSwapchain() const
	{
		return m_swapchain.get();
	}

	std::span<ImageView> Swapchain::getImageViews()
	{
		return m_imageViews;
	}

	std::span<const ImageView> Swapchain::getImageViews() const
	{
		return m_imageViews;
	}

	vk::Format Swapchain::getFormat() const
	{
		return m_imageFormat;
	}
}  // namespace trg
