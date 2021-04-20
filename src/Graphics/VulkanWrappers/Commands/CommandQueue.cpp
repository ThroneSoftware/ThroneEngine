#include "CommandQueue.h"

#include "../../Utility.h"

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

	void CommandQueue::submitCommandBuffer(SemaphorePool& waitSemaphores,
										   std::vector<vk::PipelineStageFlags> waitingStages,
										   CommandBuffer& commandBuffer,
										   SemaphorePool& semaphoresToSignal,
										   Fence& signalFence)
	{
		auto vkWaitSemaphores = toVkHandle(waitSemaphores.getAll());
		auto vkSemaphoresToSignal = toVkHandle(semaphoresToSignal.getAll());

		auto submitInfo = vk::SubmitInfo(vkWaitSemaphores, waitingStages, *commandBuffer, vkSemaphoresToSignal);
		m_queue.submit(submitInfo, *signalFence);
	}
}  // namespace trg