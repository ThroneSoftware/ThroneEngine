#include "CommandQueue.h"

namespace trg
{
	CommandQueue::CommandQueue(vk::Queue&& queue, uint32_t familyIndex)
	  : m_queue(std::move(queue))
	  , m_familyIndex(familyIndex)
	{
	}

	CommandQueue::VkHandleType& CommandQueue::getVkHandle()
	{
		return m_queue;
	}

	const CommandQueue::VkHandleType& CommandQueue::getVkHandle() const
	{
		return m_queue;
	}

	CommandQueue::VkHandleType& CommandQueue::operator*()
	{
		return getVkHandle();
	}

	const CommandQueue::VkHandleType& CommandQueue::operator*() const
	{
		return getVkHandle();
	}

	CommandQueue::VkHandleType* CommandQueue::operator->()
	{
		return &getVkHandle();
	}

	const CommandQueue::VkHandleType* CommandQueue::operator->() const
	{
		return &getVkHandle();
	}

	uint32_t CommandQueue::getFamilyIndex() const
	{
		return m_familyIndex;
	}
}  // namespace trg