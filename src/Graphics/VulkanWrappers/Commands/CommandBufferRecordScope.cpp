#include "CommandBufferRecordScope.h"

namespace trg::vkwrappers
{
	CommandBufferRecordScope::CommandBufferRecordScope(CommandBuffer& commandBuffer, vk::CommandBufferUsageFlagBits usage)
	  : m_commandBuffer(commandBuffer)
	{
		auto beginInfo = vk::CommandBufferBeginInfo(usage, nullptr);
		m_commandBuffer->begin(beginInfo);
	}

	CommandBufferRecordScope::~CommandBufferRecordScope()
	{
		m_commandBuffer->end();
	}

}  // namespace trg::vkwrappers
