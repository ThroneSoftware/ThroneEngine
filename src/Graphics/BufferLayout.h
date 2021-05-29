#pragma once

#include <fmt/format.h>

#include <array>
#include <cassert>
#include <exception>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace trg
{
	enum class BufferBlockComponentType
	{
		Scalar,
		Vec2,
		Vec3,
		Vec4,
		Mat2,
		Mat3,
		Mat4
	};

	bool isMatrice(BufferBlockComponentType componentType)
	{
		return componentType == BufferBlockComponentType::Mat2 || componentType == BufferBlockComponentType::Mat3 ||
			   componentType == BufferBlockComponentType::Mat4;
	}

	uint32_t componentTypeLocationSize(BufferBlockComponentType componentType)
	{
		switch(componentType)
		{
			case trg::BufferBlockComponentType::Scalar:
			case trg::BufferBlockComponentType::Vec2:
			case trg::BufferBlockComponentType::Vec3:
			case trg::BufferBlockComponentType::Vec4:
				return 1;
				break;
			case trg::BufferBlockComponentType::Mat2:
				return 2;
				break;
			case trg::BufferBlockComponentType::Mat3:
				return 3;
				break;
			case trg::BufferBlockComponentType::Mat4:
				return 4;
				break;
			default:
				assert(false);
				break;
		}
	}

	enum class BufferBlockValueType
	{
		Float,
		Int,
	};

	enum class StandardAttributes : uint32_t
	{
		// Enum value represents the location in the shader.
		// Reserved range : [0, 99], [100 000, 4 294 967 295]
		// User range : [100, 99 999]

		Position = 0,
		Normal = 1,
		TexCoords = 2,
		Color = 3,

		ModelMatrix = 4,
		ModelMatrix1 = 4,
		ModelMatrix2 = 5,
		ModelMatrix3 = 6,
		ModelMatrix4 = 7,

		NonStandard = std::numeric_limits<uint32_t>::max()
	};

	struct MatriceAttribute
	{
		StandardAttributes m_attribute;
		uint32_t m_maxSize;
	};

	constexpr std::array matriceAttributes = {MatriceAttribute{StandardAttributes::ModelMatrix, 4}};

	class BufferBlock
	{
		friend bool operator==(const BufferBlock& lhs, const BufferBlock& rhs);

	public:
		BufferBlock(BufferBlockComponentType componentType, BufferBlockValueType valueType, StandardAttributes attribute)
		  : m_componentType(componentType)
		  , m_valueType(valueType)
		  , m_attribute(attribute)
		{
			validateTypeSize();
		}

	private:
		void validateTypeSize()
		{
			if(isMatrice(m_componentType) && m_attribute != StandardAttributes::NonStandard)
			{
				auto found = std::find_if(matriceAttributes.begin(), matriceAttributes.end(), [this](MatriceAttribute matAttribute) {
					return matAttribute.m_attribute == m_attribute;
				});

				if(found == matriceAttributes.end())
				{
					throw std::runtime_error(fmt::format("Standard attribute matrice not found. Standard attribute: {}, Component type: {}",
														 m_attribute,
														 m_componentType));
				}
				else if(componentTypeLocationSize(m_componentType) <= found->m_maxSize)
				{
					throw std::runtime_error(
						fmt::format("Component type size must be smaller or equal as the standard attribute max size. "
									"Standard attribute: {}, Component type: {}, Component type size: {}, Standard attribute max size: {}",
									m_attribute,
									m_componentType,
									componentTypeLocationSize(m_componentType),
									found->m_maxSize));
				}
			}
		}

	private:
		BufferBlockComponentType m_componentType;
		BufferBlockValueType m_valueType;
		StandardAttributes m_attribute = StandardAttributes::NonStandard;
	};

	bool operator==(const BufferBlock& lhs, const BufferBlock& rhs)
	{
		return lhs.m_componentType == rhs.m_componentType && lhs.m_valueType == rhs.m_valueType && lhs.m_attribute == rhs.m_attribute;
	}

	bool operator!=(const BufferBlock& lhs, const BufferBlock& rhs)
	{
		return !(lhs == rhs);
	}

	struct BufferLayout
	{
		std::vector<BufferBlock> m_blocks;
	};

	bool operator==(const BufferLayout& lhs, const BufferLayout& rhs)
	{
		return lhs.m_blocks == rhs.m_blocks;
	}

	bool operator!=(const BufferLayout& lhs, const BufferLayout& rhs)
	{
		return !(lhs == rhs);
	}
}  // namespace trg