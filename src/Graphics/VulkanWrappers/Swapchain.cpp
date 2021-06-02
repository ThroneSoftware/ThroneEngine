#include "Swapchain.h"

#include "Syncronization/Fence.h"

#include <VkBootstrap.h>

namespace trg::vkwrappers
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
	  : m_device(device)
	  , m_swapchain(vk::UniqueSwapchainKHR(swapchain.swapchain, vk::ObjectDestroy(device, nullptr, vk::defaultDispatchLoaderDynamic)))
	  , m_imageViews(SwapchainPrivate::getImageViews(device, swapchain))
	  , m_imageFormat(static_cast<vk::Format>(swapchain.image_format))
	{
	}

	Swapchain::VkHandleType& Swapchain::getVkHandle()
	{
		return m_swapchain.get();
	}

	const Swapchain::VkHandleType& Swapchain::getVkHandle() const
	{
		return m_swapchain.get();
	}

	Swapchain::VkHandleType& Swapchain::operator*()
	{
		return getVkHandle();
	}

	const Swapchain::VkHandleType& Swapchain::operator*() const
	{
		return getVkHandle();
	}

	Swapchain::VkHandleType* Swapchain::operator->()
	{
		return &getVkHandle();
	}

	const Swapchain::VkHandleType* Swapchain::operator->() const
	{
		return &getVkHandle();
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

	std::uint32_t Swapchain::acquireImage(Semaphore& acquireNextImageSemaphore)
	{
		auto imageIndex =
			m_device.acquireNextImageKHR(*m_swapchain, std::numeric_limits<std::uint64_t>::max(), *acquireNextImageSemaphore, vk::Fence());
		return imageIndex.value;
	}
}  // namespace trg::vkwrappers
