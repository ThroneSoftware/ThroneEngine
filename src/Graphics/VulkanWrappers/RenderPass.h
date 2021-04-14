#pragma once

#include <Vulkan/vulkan.hpp>

namespace trg
{
	class RenderPass
	{
	public:
		using VkHandleType = vk::Framebuffer;

	public:
		RenderPass(vk::Device& device, vk::Format swapchainFormat);

		vk::RenderPass& getVkHandle();
		const vk::RenderPass& getVkHandle() const;

	private:
		vk::UniqueRenderPass m_renderPass;
	};
}  // namespace trg