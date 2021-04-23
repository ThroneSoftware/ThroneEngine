#pragma once

#include "../../VulkanContext.h"
#include "../FrameBuffer.h"
#include "../RenderPass.h"
#include "CommandBuffer.h"

#include <glm/glm.hpp>

namespace trg
{
	class RenderPassRecordScope
	{
	public:
		RenderPassRecordScope(CommandBuffer& commandBuffer,
							  VulkanContext& vkContext,
							  RenderPass& renderPass,
							  FrameBuffer& frameBuffer,
							  glm::vec3 clearColor);

		RenderPassRecordScope(const RenderPassRecordScope& other) = default;
		RenderPassRecordScope& operator=(const RenderPassRecordScope& other) = default;

		RenderPassRecordScope(RenderPassRecordScope&& other) = default;
		RenderPassRecordScope& operator=(RenderPassRecordScope&& other) = default;

		~RenderPassRecordScope();

	private:
		CommandBuffer& m_commandBuffer;
	};
}  // namespace trg