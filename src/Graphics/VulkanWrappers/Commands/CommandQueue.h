#pragma once

#include "../Syncronization/Fence.h"
#include "../Syncronization/SemaphorePool.h"
#include "CommandBuffer.h"
#include "CommandPool.h"

#include <Vulkan/vulkan.hpp>

namespace trg::vkwrappers
{
	class CommandQueue
	{
	public:
		using VkHandleType = vk::Queue;

	public:
		CommandQueue() = default;
		CommandQueue(vk::Device& device, vk::Queue&& queue, uint32_t familyIndex);

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

		template <typename Func>
		void immediateSubmit(Func&& func)
		{
			func(m_immediateCommandBuffer);

			submitCommandBuffer({}, {}, m_immediateCommandBuffer, {}, m_immediateFence);

			m_immediateFence.wait();
			m_immediateFence.reset();
		}

	private:
		vk::Queue m_queue;
		uint32_t m_familyIndex = 0;

		Fence m_immediateFence;
		CommandPool m_immedidateCommandPool;
		CommandBuffer& m_immediateCommandBuffer;
	};
}  // namespace trg::vkwrappers