#pragma once

#include "Memory/ImageView.h"
#include "RenderPass.h"

#include <Vulkan/vulkan.hpp>

#include <span>

namespace trg
{
	class FrameBuffer
	{
	public:
		using VkHandleType = vk::Framebuffer;

	public:
		FrameBuffer(vk::Device& device,
					RenderPass& renderPass,
					std::span<ImageView> attachments,
					vk::Extent2D frameBufferSize,
					uint32_t layers);

		vk::Framebuffer& getVkHandle();
		const vk::Framebuffer& getVkHandle() const;

	private:
		vk::UniqueFramebuffer m_frameBuffer;
	};
}  // namespace trg