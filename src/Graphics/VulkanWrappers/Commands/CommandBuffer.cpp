#include "CommandBuffer.h"

#include "CommandPool.h"

#include <gsl/gsl>

namespace trg
{
	CommandBuffer::CommandBuffer(vk::UniqueCommandBuffer commandBuffer)
	  : m_commandBuffer(std::move(commandBuffer))
	{
	}

	void CommandBuffer::createCommandBuffers(vk::Device& device,
											 CommandPool& commandPool,
											 vk::CommandBufferLevel commandBufferLevel,
											 std::span<CommandBuffer> commandBuffersToAllocate)
	{
		auto allocateInfo =
			vk::CommandBufferAllocateInfo(*commandPool, commandBufferLevel, gsl::narrow<uint32_t>(commandBuffersToAllocate.size()));

		auto commandBuffers = device.allocateCommandBuffersUnique(allocateInfo);

		for(size_t i = 0; i < commandBuffers.size(); ++i)
		{
			commandBuffersToAllocate[i] = CommandBuffer(std::move(commandBuffers[i]));
		}
	}

	CommandBuffer::VkHandleType& CommandBuffer::getVkHandle()
	{
		return m_commandBuffer.get();
	}

	const CommandBuffer::VkHandleType& CommandBuffer::getVkHandle() const
	{
		return m_commandBuffer.get();
	}

	CommandBuffer::VkHandleType& CommandBuffer::operator*()
	{
		return getVkHandle();
	}

	const CommandBuffer::VkHandleType& CommandBuffer::operator*() const
	{
		return getVkHandle();
	}

	CommandBuffer::VkHandleType* CommandBuffer::operator->()
	{
		return &getVkHandle();
	}

	const CommandBuffer::VkHandleType* CommandBuffer::operator->() const
	{
		return &getVkHandle();
	}
}  // namespace trg