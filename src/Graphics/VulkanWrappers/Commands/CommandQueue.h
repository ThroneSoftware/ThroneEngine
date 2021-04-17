#pragma once

#include <Vulkan/vulkan.hpp>

namespace trg
{
	class CommandQueue
	{
	public:
		using VkHandleType = vk::Queue;

	public:
		CommandQueue(vk::Queue&& queue, uint32_t familyIndex);

		VkHandleType& getVkHandle();
		const VkHandleType& getVkHandle() const;

		VkHandleType& operator*();
		const VkHandleType& operator*() const;

		VkHandleType* operator->();
		const VkHandleType* operator->() const;

		const uint32_t m_familyIndex = 0;

	private:
		vk::Queue m_queue;
	};
}  // namespace trg