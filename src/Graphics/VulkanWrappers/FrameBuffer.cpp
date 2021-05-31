#include "FrameBuffer.h"

#include "../Utility.h"

namespace trg::vkwrappers
{
	namespace FrameBufferPrivate
	{
		auto makeFrameBuffer(vk::Device& device,
							 RenderPass& renderPass,
							 std::span<vk::ImageView> attachments,
							 vk::Extent2D frameBufferSize,
							 uint32_t layers)
		{
			auto frameBufferCreateInfo =
				vk::FramebufferCreateInfo({}, *renderPass, attachments, frameBufferSize.width, frameBufferSize.height, layers);


			return device.createFramebufferUnique(frameBufferCreateInfo);
		}
	}  // namespace FrameBufferPrivate


	FrameBuffer::FrameBuffer(vk::Device& device,
							 RenderPass& renderPass,
							 std::span<vk::ImageView> attachments,
							 vk::Extent2D frameBufferSize,
							 uint32_t layers)
	  : m_frameBuffer(FrameBufferPrivate::makeFrameBuffer(device, renderPass, attachments, frameBufferSize, layers))
	{
	}

	FrameBuffer::VkHandleType& FrameBuffer::getVkHandle()
	{
		return m_frameBuffer.get();
	}

	const FrameBuffer::VkHandleType& FrameBuffer::getVkHandle() const
	{
		return m_frameBuffer.get();
	}

	FrameBuffer::VkHandleType& FrameBuffer::operator*()
	{
		return getVkHandle();
	}

	const FrameBuffer::VkHandleType& FrameBuffer::operator*() const
	{
		return getVkHandle();
	}

	FrameBuffer::VkHandleType* FrameBuffer::operator->()
	{
		return &getVkHandle();
	}

	const FrameBuffer::VkHandleType* FrameBuffer::operator->() const
	{
		return &getVkHandle();
	}
}  // namespace trg::vkwrappers