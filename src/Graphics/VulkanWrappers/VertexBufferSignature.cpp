#include "VertexBufferSignature.h"

namespace trg::vkwrappers
{
	namespace VertexBufferSignaturePrivate
	{
		vk::Format toVkFormat(BufferBlockComponentType componentType, BufferBlockValueType valueType)
		{
			switch(componentType)
			{
				case trg::BufferBlockComponentType::Scalar:
					switch(valueType)
					{
						case trg::BufferBlockValueType::Float:
							return vk::Format::eR32Sfloat;
						case trg::BufferBlockValueType::Uint32:
							return vk::Format::eR32Uint;
					}
				case trg::BufferBlockComponentType::Vec2:
				case trg::BufferBlockComponentType::Mat2:
					switch(valueType)
					{
						case trg::BufferBlockValueType::Float:
							return vk::Format::eR32G32Sfloat;
						case trg::BufferBlockValueType::Uint32:
							return vk::Format::eR32G32Uint;
					}
				case trg::BufferBlockComponentType::Vec3:
				case trg::BufferBlockComponentType::Mat3:
					switch(valueType)
					{
						case trg::BufferBlockValueType::Float:
							return vk::Format::eR32G32B32Sfloat;
						case trg::BufferBlockValueType::Uint32:
							return vk::Format::eR32G32B32Uint;
					}
				case trg::BufferBlockComponentType::Vec4:
				case trg::BufferBlockComponentType::Mat4:
					switch(valueType)
					{
						case trg::BufferBlockValueType::Float:
							return vk::Format::eR32G32B32A32Sfloat;
						case trg::BufferBlockValueType::Uint32:
							return vk::Format::eR32G32B32A32Uint;
					}
			}

			assert(false);
			throw std::runtime_error("Invalid range");
		}

		std::vector<VertexBufferBlock> toVertexBufferBlocks(const BufferLayout& bufferLayout)
		{
			std::vector<VertexBufferBlock> bufferBlocks;
			bufferBlocks.reserve(bufferLayout.m_blocks.size());

			for(auto& block: bufferLayout.m_blocks)
			{
				auto count = componentTypeLocationSize(block.getComponentType());
				auto typeSize = block.totalByteSize() / count;

				bufferBlocks.emplace_back(VertexBufferBlock(typeSize,
															count,
															static_cast<uint32_t>(block.getStandardAttribute()),
															toVkFormat(block.getComponentType(), block.getValueType())));
			}

			return bufferBlocks;
		}
	}  // namespace VertexBufferSignaturePrivate


	VertexBufferBlock::VertexBufferBlock(uint32_t typeSize, uint32_t count, uint32_t shaderLocation, vk::Format format)
	  : m_typeSize(typeSize)
	  , m_count(count)
	  , m_shaderLocation(shaderLocation)
	  , m_format(format)
	{
	}

	VertexBufferSignature::VertexBufferSignature(uint32_t bindingIndex, vk::VertexInputRate inputRate, const BufferLayout& bufferLayout)
	  : m_bufferLayout(bufferLayout)
	  , m_bindingIndex(bindingIndex)
	  , m_inputRate(inputRate)
	  , m_blocks(VertexBufferSignaturePrivate::toVertexBufferBlocks(m_bufferLayout))
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

	bool operator==(const VertexBufferSignature& lhs, const VertexBufferSignature& rhs)
	{
		return lhs.m_bufferLayout == rhs.m_bufferLayout;
	}

	bool operator!=(const VertexBufferSignature& lhs, const VertexBufferSignature& rhs)
	{
		return !(lhs == rhs);
	}
}  // namespace trg::vkwrappers