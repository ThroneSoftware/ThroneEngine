#include "VertexBufferSignature.h"

namespace trg::vkwrappers
{
	VertexBufferBlock::VertexBufferBlock(uint32_t typeSize, uint32_t count, uint32_t shaderLocation, vk::Format format)
	  : m_typeSize(typeSize)
	  , m_count(count)
	  , m_shaderLocation(shaderLocation)
	  , m_format(format)
	{
	}

	VertexBufferSignature::VertexBufferSignature(uint32_t bindingIndex,
												 vk::VertexInputRate inputRate,
												 std::vector<VertexBufferBlock> blocks)
	  : m_bindingIndex(bindingIndex)
	  , m_inputRate(inputRate)
	  , m_blocks(std::move(blocks))
	{
	}

	VertexInputDescriptions VertexBufferSignature::getInputBindingDescription(std::span<const VertexBufferSignature> signatures)
	{
		VertexInputDescriptions inputDescriptions = {};

		for(const auto& signature: signatures)
		{
			uint32_t offset = 0;
			for(const auto& block: signature.m_blocks)
			{
				for(size_t i = 0; i < block.m_count; ++i)
				{
					inputDescriptions.m_attributeDescription.emplace_back(block.m_shaderLocation,
																		  signature.m_bindingIndex,
																		  block.m_format,
																		  offset);

					offset += block.m_typeSize;
				}
			}

			inputDescriptions.m_bindingDescription.emplace_back(signature.m_bindingIndex, offset, signature.m_inputRate);
		}

		return inputDescriptions;
	}
}  // namespace trg::vkwrappers