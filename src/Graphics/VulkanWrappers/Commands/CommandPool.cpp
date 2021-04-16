#include "CommandPool.h"

#include "CommandQueue.h"

#include <span>

namespace trg
{
	namespace CommandPoolPrivate
	{
		auto makeCommandPool(vk::Device& device, const CommandQueue& commandQueue, vk::CommandPoolCreateFlagBits flags)
		{
			auto createInfo = vk::CommandPoolCreateInfo(flags, commandQueue.m_familyIndex);
			return device.createCommandPoolUnique(createInfo);
		}
	}  // namespace CommandPoolPrivate

	CommandPool::CommandPool(vk::Device& device,
							 const CommandQueue& commandQueue,
							 vk::CommandPoolCreateFlagBits flags,
							 int numberOfCommandBuffers,
							 vk::CommandBufferLevel commandBufferLevel)
	  : m_device(device)
	  , m_commandPool(CommandPoolPrivate::makeCommandPool(device, commandQueue, flags))
	{
		addCommandBuffers(numberOfCommandBuffers, commandBufferLevel);
	}

	CommandPool::VkHandleType& CommandPool::getVkHandle()
	{
		return m_commandPool.get();
	}

	const CommandPool::VkHandleType& CommandPool::getVkHandle() const
	{
		return m_commandPool.get();
	}

	CommandPool::VkHandleType& CommandPool::operator*()
	{
		return getVkHandle();
	}

	const CommandPool::VkHandleType& CommandPool::operator*() const
	{
		return getVkHandle();
	}

	CommandPool::VkHandleType* CommandPool::operator->()
	{
		return &getVkHandle();
	}

	const CommandPool::VkHandleType* CommandPool::operator->() const
	{
		return &getVkHandle();
	}

	void CommandPool::addCommandBuffers(int numberOfCommandBuffers, vk::CommandBufferLevel commandBufferLevel)
	{
		assert(numberOfCommandBuffers != 0);

		auto previousSize = m_commandBuffers.size();
		m_commandBuffers.resize(previousSize + numberOfCommandBuffers);
		CommandBuffer::createCommandBuffers(m_device,
											*this,
											commandBufferLevel,
											std::span(m_commandBuffers.end() - numberOfCommandBuffers, numberOfCommandBuffers));
	}
}  // namespace trg