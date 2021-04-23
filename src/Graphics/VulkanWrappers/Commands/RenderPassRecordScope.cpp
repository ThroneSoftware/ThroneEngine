#include "RenderPassRecordScope.h"

namespace trg
{
	RenderPassRecordScope::RenderPassRecordScope(CommandBuffer& commandBuffer,
												 VulkanContext& vkContext,
												 RenderPass& renderPass,
												 FrameBuffer& frameBuffer,
												 glm::vec3 clearColor)
	  : m_commandBuffer(commandBuffer)
	{
		auto subpassContents = vk::SubpassContents::eInline;

		auto vkClearColor = vk::ClearValue(vk::ClearColorValue(std::array{clearColor.r, clearColor.g, clearColor.b, 1.0f}));
		auto vlClearDepth = vk::ClearValue(vk::ClearDepthStencilValue(1.0f));

		auto clearValues = std::vector<vk::ClearValue>{vkClearColor, vlClearDepth};
		auto beginInfo = vk::RenderPassBeginInfo(*renderPass, *frameBuffer, vk::Rect2D({0, 0}, vkContext.m_swapchainExtent), clearValues);

		m_commandBuffer->beginRenderPass(beginInfo, subpassContents);
	}

	RenderPassRecordScope::~RenderPassRecordScope()
	{
		m_commandBuffer->endRenderPass();
	}

}  // namespace trg
