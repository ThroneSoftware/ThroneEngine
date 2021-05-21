#pragma once

#include "../Commands/CommandBuffer.h"

namespace trg
{
	// concept IsBindable

	struct BindableBindInfo
	{
		trg::CommandBuffer& commandBuffer;
	};
}  // namespace trg