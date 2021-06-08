#include "Mesh.h"

#include <Utilities/Utility.h>

namespace trg
{
	namespace MeshPrivate
	{
		void getVertexCount()
		{
		}

		std::vector<float> zipAttributes(const std::vector<MeshAttribute>& attributes)
		{
			assert(std::adjacent_find(attributes.cbegin(),
									  attributes.cend(),
									  [](const MeshAttribute& attribute1, const MeshAttribute& attribute2) {
										  return attribute1.getVertexCount() != attribute2.getVertexCount();
									  }) == attributes.end());

			std::vector<float> zippedData;

			if(!attributes.empty())
			{
				std::size_t vertexCount = attributes.front().getVertexCount();

				std::size_t completeDataSize = 0;
				for(const auto& attribute: attributes)
				{
					completeDataSize += attribute.m_data.size();
				}
				zippedData.reserve(completeDataSize);

				for(size_t vertexIndex = 0; vertexIndex < vertexCount; ++vertexIndex)
				{
					for(const auto& attribute: attributes)
					{
						auto fullLength = componentTypeFullLength(attribute.m_bufferBlock.getComponentType());
						for(size_t arrayIndex = 0; arrayIndex < fullLength; ++arrayIndex)
						{
							zippedData.emplace_back(attribute.m_data[(vertexIndex * fullLength) + arrayIndex]);
						}
					}
				}
			}

			return zippedData;
		}
	}  // namespace MeshPrivate

	Mesh::Mesh(const std::string& name,
			   std::vector<MeshAttribute> attributes,
			   std::vector<uint16_t> indices,
			   const MaterialInfo& materialInfo)
	  : m_name(name)
	  , m_attributes(std::move(attributes))
	  , m_zippedAttributes(MeshPrivate::zipAttributes(m_attributes))
	  , m_indices(std::move(indices))
	  , m_materialInfo(materialInfo)
	{
	}

	std::span<const float> Mesh::getAttributeData() const
	{
		return m_zippedAttributes;
	}

	std::span<const uint16_t> Mesh::getIndexData() const
	{
		return m_indices;
	}
}  // namespace trg