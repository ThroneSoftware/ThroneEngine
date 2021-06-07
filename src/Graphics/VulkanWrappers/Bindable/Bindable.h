#pragma once

#include "../Commands/CommandBuffer.h"

namespace trg::vkwrappers
{
	// concept IsBindable

	struct BindableBindInfo
	{
		CommandBuffer& m_commandBuffer;
		vk::PipelineLayout& m_pipelineLayout;
	};
}  // namespace trg::vkwrappers