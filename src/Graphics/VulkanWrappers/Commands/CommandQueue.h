#pragma once

#include "../Syncronization/Fence.h"
#include "../Syncronization/SemaphorePool.h"
#include "CommandBuffer.h"

#include <Vulkan/vulkan.hpp>

namespace trg::vkwrappers
{
	class CommandQueue
	{
	public:
		using VkHandleType = vk::Queue;

	public:
		CommandQueue() = default;
		CommandQueue(vk::Queue&& queue, uint32_t familyIndex);

		CommandQueue(const CommandQueue& other) = default;
		CommandQueue& operator=(const CommandQueue& other) = default;

		CommandQueue(CommandQueue&& other) = default;
		CommandQueue& operator=(CommandQueue&& other) = default;

		~CommandQueue() = default;

		VkHandleType& getVkHandle();
		const VkHandleType& getVkHandle() const;

		VkHandleType& operator*();
		const VkHandleType& operator*() const;

		VkHandleType* operator->();
		const VkHandleType* operator->() const;

		uint32_t getFamilyIndex() const;

		void submitCommandBuffer(std::span<vk::Semaphore> waitSemaphores,
								 std::vector<vk::PipelineStageFlags> waitingStages,
								 CommandBuffer& commandBuffer,
								 std::span<Semaphore> semaphoresToSignal,
								 Fence& signalFence);

	private:
		vk::Queue m_queue;
		uint32_t m_familyIndex = 0;
	};
}  // namespace trg::vkwrappers