#pragma once

#include "../BufferLayout.h"
#include "../Materials/MaterialInfo.h"

#include <optional>
#include <span>
#include <string>
#include <vector>

namespace trg
{
	struct MeshAttribute
	{
		std::vector<float> m_data;
		BufferBlock m_bufferBlock;

		auto getVertexCount() const
		{
			return m_data.size() / componentTypeFullLength(m_bufferBlock.getComponentType());
		}
	};

	class Mesh
	{
	public:
		Mesh(const std::string& name,
			 std::vector<MeshAttribute> attributes,
			 std::vector<uint16_t> indices,
			 const MaterialInfo& materialInfo);

		std::span<const float> getAttributeData() const;
		std::span<const uint16_t> getIndexData() const;

		const MaterialInfo& getMaterialInfo() const;

	private:
		std::string m_name;

		std::vector<MeshAttribute> m_attributes;
		std::vector<float> m_zippedAttributes;

		std::vector<uint16_t> m_indices;

		const MaterialInfo& m_materialInfo;
	};
}  // namespace trg