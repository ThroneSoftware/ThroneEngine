#pragma once

#include "../BufferLayout.h"

#include <Vulkan/vulkan.hpp>

namespace trg::vkwrappers
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
		friend bool operator==(const VertexBufferSignature& lhs, const VertexBufferSignature& rhs);

	public:
		VertexBufferSignature(uint32_t bindingIndex, vk::VertexInputRate inputRate, const BufferLayout& bufferLayout);

		//[[deprecated]]
		VertexBufferSignature(uint32_t bindingIndex, vk::VertexInputRate inputRate, std::vector<VertexBufferBlock> blocks);

		static VertexInputDescriptions getInputBindingDescription(std::span<const VertexBufferSignature> signatures);

	private:
		BufferLayout m_bufferLayout;

		uint32_t m_bindingIndex;
		vk::VertexInputRate m_inputRate;

		std::vector<VertexBufferBlock> m_blocks;
	};

	bool operator==(const VertexBufferSignature& lhs, const VertexBufferSignature& rhs);
	bool operator!=(const VertexBufferSignature& lhs, const VertexBufferSignature& rhs);
}  // namespace trg::vkwrappers