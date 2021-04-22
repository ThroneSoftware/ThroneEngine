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
					std::span<vk::ImageView> attachments,
					vk::Extent2D frameBufferSize,
					uint32_t layers);

		VkHandleType& getVkHandle();
		const VkHandleType& getVkHandle() const;

		VkHandleType& operator*();
		const VkHandleType& operator*() const;

		VkHandleType* operator->();
		const VkHandleType* operator->() const;

	private:
		vk::UniqueFramebuffer m_frameBuffer;
	};
}  // namespace trg