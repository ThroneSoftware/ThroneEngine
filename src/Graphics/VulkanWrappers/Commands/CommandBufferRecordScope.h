#pragma once

#include "CommandBuffer.h"

namespace trg::vkwrappers
{
	class CommandBufferRecordScope
	{
	public:
		CommandBufferRecordScope(CommandBuffer& commandBuffer, vk::CommandBufferUsageFlagBits usage);

		CommandBufferRecordScope(const CommandBufferRecordScope& other) = delete;
		CommandBufferRecordScope& operator=(const CommandBufferRecordScope& other) = delete;

		CommandBufferRecordScope(CommandBufferRecordScope&& other) = delete;
		CommandBufferRecordScope& operator=(CommandBufferRecordScope&& other) = delete;

		~CommandBufferRecordScope();

	private:
		CommandBuffer& m_commandBuffer;
	};
}  // namespace trg::vkwrappers