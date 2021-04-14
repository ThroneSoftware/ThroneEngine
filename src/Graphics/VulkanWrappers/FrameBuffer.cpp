#include "FrameBuffer.h"

#include "../Utility.h"

namespace trg
{
	namespace FrameBufferPrivate
	{
		auto makeFrameBuffer(vk::Device& device,
							 RenderPass& renderPass,
							 std::span<ImageView> attachments,
							 vk::Extent2D frameBufferSize,
							 uint32_t layers)
		{
			auto vkAttachments = toVkHandle(attachments);

			auto frameBufferCreateInfo = vk::FramebufferCreateInfo({},
																   renderPass.getVkHandle(),
																   vkAttachments,
																   frameBufferSize.width,
																   frameBufferSize.height,
																   layers);


			return device.createFramebufferUnique(frameBufferCreateInfo);
		}
	}  // namespace FrameBufferPrivate


	FrameBuffer::FrameBuffer(vk::Device& device,
							 RenderPass& renderPass,
							 std::span<ImageView> attachments,
							 vk::Extent2D frameBufferSize,
							 uint32_t layers)
	  : m_frameBuffer(FrameBufferPrivate::makeFrameBuffer(device, renderPass, attachments, frameBufferSize, layers))
	{
	}

	vk::Framebuffer& FrameBuffer::getVkHandle()
	{
		return m_frameBuffer.get();
	}

	const vk::Framebuffer& FrameBuffer::getVkHandle() const
	{
		return m_frameBuffer.get();
	}
}  // namespace trg