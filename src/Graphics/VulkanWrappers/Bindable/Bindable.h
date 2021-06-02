#pragma once

#include "../Commands/CommandBuffer.h"

namespace trg::vkwrappers
{
	// concept IsBindable

	struct BindableBindInfo
	{
		CommandBuffer& commandBuffer;
	};
}  // namespace trg::vkwrappers