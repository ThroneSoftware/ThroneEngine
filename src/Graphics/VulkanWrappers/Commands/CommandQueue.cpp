#include "CommandQueue.h"

#include "../../Utility.h"

namespace trg::vkwrappers
{
	CommandQueue::CommandQueue(vk::Device& device, vk::Queue&& queue, uint32_t familyIndex)
	  : m_queue(std::move(queue))
	  , m_familyIndex(familyIndex)
	  , m_immediateFence(device)
	  , m_immedidateCommandPool(device,
								*this,
								vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
								1 /*numberOfCommandBuffers*/,
								vk::CommandBufferLevel::ePrimary)
	  , m_immediateCommandBuffer(m_immedidateCommandPool.getAll()[0])
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

	void CommandQueue::submitCommandBuffer(std::span<vk::Semaphore> waitSemaphores,
										   std::vector<vk::PipelineStageFlags> waitingStages,
										   CommandBuffer& commandBuffer,
										   std::span<Semaphore> semaphoresToSignal,
										   Fence& signalFence)
	{
		auto vkSemaphoresToSignal = toVkHandle(semaphoresToSignal);

		auto submitInfo = vk::SubmitInfo(waitSemaphores, waitingStages, *commandBuffer, vkSemaphoresToSignal);
		m_queue.submit(submitInfo, *signalFence);
	}
}  // namespace trg::vkwrappers