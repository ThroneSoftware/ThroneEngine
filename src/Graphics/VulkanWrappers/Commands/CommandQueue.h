#pragma once

#include <Vulkan/vulkan.hpp>

namespace trg
{
	class CommandQueue
	{
	public:
		CommandQueue();

		const uint32_t m_familyIndex = 0;

	private:
		vk::Queue m_queue;
	};
}  // namespace trg