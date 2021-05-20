#pragma once

#include <Vulkan/vulkan.hpp>

namespace trg
{
	class VertexBufferBlock
	{
		friend class VertexBufferSignature;

	public:
		VertexBufferBlock(uint32_t typeSize, uint32_t count, uint32_t shaderLocation, vk::Format format);

	private:
		uint32_t m_typeSize;
		uint32_t m_count;
		uint32_t m_shaderLocation;
		vk::Format m_format;
	};

	struct VertexInputDescriptions
	{
		std::vector<vk::VertexInputAttributeDescription> m_attributeDescription;
		std::vector<vk::VertexInputBindingDescription> m_bindingDescription;
	};

	class VertexBufferSignature
	{
	public:
		VertexBufferSignature(uint32_t bindingIndex, vk::VertexInputRate inputRate, std::vector<VertexBufferBlock> blocks);

		static VertexInputDescriptions getInputBindingDescription(std::span<const VertexBufferSignature> signatures);

	private:
		uint32_t m_bindingIndex;
		vk::VertexInputRate m_inputRate;
		std::vector<VertexBufferBlock> m_blocks;
	};
}  // namespace trg