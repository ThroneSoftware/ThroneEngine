#pragma once

#include "../Bindable/Bindable.h"
#include "../Descriptors/DescriptorSetLayout.h"
#include "../RenderPass.h"
#include "../Shaders/Shader.h"
#include "../VertexBufferSignature.h"

#include <Vulkan/vulkan.hpp>

namespace trg
{
	class GraphicsPipeline
	{
	public:
		GraphicsPipeline(vk::Device& device,
						 std::span<const trg::DescriptorSetLayout> descriptorSetLayouts,
						 std::vector<vk::PushConstantRange> pushContants,
						 std::span<const Shader> shaders,
						 std::span<const VertexBufferSignature> bufferSignatures,
						 vk::PipelineInputAssemblyStateCreateInfo* inputAssemblyState,
						 vk::PipelineTessellationStateCreateInfo* tesselationState,
						 vk::PipelineViewportStateCreateInfo* viewportState,
						 vk::PipelineRasterizationStateCreateInfo* rasterizationState,
						 vk::PipelineMultisampleStateCreateInfo* multisampleState,
						 vk::PipelineDepthStencilStateCreateInfo* depthStencilState,
						 vk::PipelineColorBlendStateCreateInfo* colorBlendState,
						 vk::PipelineDynamicStateCreateInfo* dynamicState,
						 const trg::RenderPass& renderPass);

		void bind(BindableBindInfo& bindInfo);

	private:
		vk::UniquePipelineLayout m_layout;
		vk::UniquePipeline m_pipeline;
	};
}  // namespace trg