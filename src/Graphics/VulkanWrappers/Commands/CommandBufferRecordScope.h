#pragma once

#include "CommandBuffer.h"

namespace trg
{
	class CommandBufferRecordScope
	{
	public:
		CommandBufferRecordScope(CommandBuffer& commandBuffer, vk::CommandBufferUsageFlagBits usage);

		CommandBufferRecordScope(const CommandBufferRecordScope& other) = default;
		CommandBufferRecordScope& operator=(const CommandBufferRecordScope & other) = default;

		CommandBufferRecordScope(CommandBufferRecordScope && other) = default;
		CommandBufferRecordScope& operator=(CommandBufferRecordScope && other) = default;

		~CommandBufferRecordScope();

	private:
		CommandBuffer& m_commandBuffer;
	};
}