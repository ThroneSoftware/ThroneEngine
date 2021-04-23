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

		RenderPassRecordScope(const RenderPassRecordScope& other) = delete;
		RenderPassRecordScope& operator=(const RenderPassRecordScope& other) = delete;

		RenderPassRecordScope(RenderPassRecordScope&& other) = delete;
		RenderPassRecordScope& operator=(RenderPassRecordScope&& other) = delete;

		~RenderPassRecordScope();

	private:
		CommandBuffer& m_commandBuffer;
	};
}  // namespace trg