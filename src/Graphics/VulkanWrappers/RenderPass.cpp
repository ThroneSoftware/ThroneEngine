#include "RenderPass.h"

namespace trg
{
	RenderPass::RenderPass(vk::Device& device, vk::Format swapchainFormat)
	{
		auto colorAttachment = vk::AttachmentDescription({},
														 swapchainFormat,
														 vk::SampleCountFlagBits::e1,
														 vk::AttachmentLoadOp::eClear,
														 vk::AttachmentStoreOp::eStore,
														 vk::AttachmentLoadOp::eDontCare,
														 vk::AttachmentStoreOp::eDontCare,
														 vk::ImageLayout::eUndefined,
														 vk::ImageLayout::ePresentSrcKHR);

		auto depthAttachment = vk::AttachmentDescription({},
														 vk::Format::eD32Sfloat,
														 vk::SampleCountFlagBits::e1,
														 vk::AttachmentLoadOp::eClear,
														 vk::AttachmentStoreOp::eStore,
														 vk::AttachmentLoadOp::eDontCare,
														 vk::AttachmentStoreOp::eDontCare,
														 vk::ImageLayout::eUndefined,
														 vk::ImageLayout::eDepthAttachmentOptimal);

		auto colorAttachmentRef = vk::AttachmentReference(0, vk::ImageLayout::eColorAttachmentOptimal);

		auto depthAttachmentRef = vk::AttachmentReference(1, vk::ImageLayout::eDepthAttachmentOptimal);

		auto colorAttachments = {colorAttachmentRef};
		decltype(colorAttachments) inputAttachments = {};
		decltype(colorAttachments) resolveAttachments = {};
		auto subpassDescription = vk::SubpassDescription({},
														 vk::PipelineBindPoint::eGraphics,
														 inputAttachments,
														 colorAttachments,
														 resolveAttachments,
														 &depthAttachmentRef);


		auto dependency1 = vk::SubpassDependency(VK_SUBPASS_EXTERNAL,
												 0,
												 vk::PipelineStageFlagBits::eLateFragmentTests,
												 vk::PipelineStageFlagBits::eFragmentShader,
												 vk::AccessFlagBits::eDepthStencilAttachmentWrite,
												 vk::AccessFlagBits::eShaderRead,
												 vk::DependencyFlagBits::eByRegion);

		auto dependency2 = vk::SubpassDependency(0,
												 VK_SUBPASS_EXTERNAL,
												 vk::PipelineStageFlagBits::eColorAttachmentOutput,
												 vk::PipelineStageFlagBits::eTopOfPipe,
												 vk::AccessFlagBits::eColorAttachmentWrite,
												 vk::AccessFlagBits::eMemoryRead,
												 vk::DependencyFlagBits::eByRegion);


		auto attachments = {colorAttachment, depthAttachment};
		auto subpassDescriptions = {subpassDescription};
		auto subpassDependencies = {dependency1, dependency2};
		vk::RenderPassCreateInfo createInfo = vk::RenderPassCreateInfo({}, attachments, subpassDescriptions, subpassDependencies);


		m_renderPass = device.createRenderPassUnique(createInfo);
	}
}  // namespace trg