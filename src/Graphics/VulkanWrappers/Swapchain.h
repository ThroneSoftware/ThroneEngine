#pragma once

#include <Vulkan/vulkan.hpp>


namespace vkb
{
	struct Swapchain;
}

namespace trg
{
	class Swapchain
	{
	public:
		Swapchain() = default;
		Swapchain(vk::Device& device, vkb::Swapchain& swapchain);

		Swapchain(const Swapchain& other) = delete;
		Swapchain& operator=(const Swapchain& other) = delete;

		Swapchain(Swapchain&& other) = delete;
		Swapchain& operator=(Swapchain&& other) = delete;

		~Swapchain();

		void reset();

		vk::SwapchainKHR& getSwapchain();
		const vk::SwapchainKHR& getSwapchain() const;

	private:
		vk::Device& m_device;

		vk::SwapchainKHR m_swapchain;

		vk::Format m_imageFormat;
	};
}  // namespace trg