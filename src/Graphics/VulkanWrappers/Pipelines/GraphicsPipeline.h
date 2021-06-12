#pragma once

#include "../Bindable/Bindable.h"
#include "../Descriptors/DescriptorSetLayout.h"
#include "../RenderPass.h"
#include "../Shaders/Shader.h"
#include "../VertexBufferSignature.h"

#include <Vulkan/vulkan.hpp>

namespace trg::vkwrappers
{
	class GraphicsPipeline
	{
	public:
		GraphicsPipeline(vk::Device& device,
						 const std::vector<std::reference_wrapper<const DescriptorSetLayout>>& descriptorSetLayouts,
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
						 const RenderPass& renderPass);

		void bind(BindableBindInfo& bindInfo);

		vk::PipelineLayout& getLayout();

	private:
		vk::UniquePipelineLayout m_layout;
		vk::UniquePipeline m_pipeline;
	};
}  // namespace trg::vkwrappers