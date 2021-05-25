#pragma once

#include <vector>

namespace trg
{
	enum BufferBlockComponentType
	{
		Scalar,
		Vec2,
		Vec3,
		Vec4,
		Mat2,
		Mat3,
		Mat4
	};

	enum BufferBlockValueType
	{
		Float,
		Int,
	};

	struct BufferBlock
	{
		BufferBlockComponentType m_componentType;
		BufferBlockValueType m_valueType;
	};

	bool operator==(const BufferBlock& lhs, const BufferBlock& rhs)
	{
		return lhs.m_componentType == rhs.m_componentType && lhs.m_valueType == rhs.m_valueType;
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