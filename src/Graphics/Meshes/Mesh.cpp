#include "Mesh.h"

#include <Utilities/Utility.h>

namespace trg
{
	namespace MeshPrivate
	{
		std::vector<float> zipAttributes(const std::vector<MeshAttribute>& attributes)
		{
			assert(std::adjacent_find(values.cbegin(), values.cend(), [](const MeshAttribute& attribute1, const MeshAttribute& attribute2) {
					   return attribute1.m_data.size() != attribute2.m_data.size();
				   }) == values.end());

			std::vector<float> zippedData;

			if(!attributes.empty())
			{
				std::size_t dataSize = attributes.front().m_data.size();

				zippedData.reserve(dataSize * attributes.size());

				for(size_t i = 0; i < dataSize; ++i)
				{
					for(const auto& attribute: attributes)
					{
						zippedData.emplace_back(attribute.m_data[dataSize]);
					}
				}
			}

			return zippedData;
		}
	}  // namespace MeshPrivate

	Mesh::Mesh(const std::string& name,
			   std::vector<MeshAttribute> attributes,
			   std::vector<uint16_t> indices,
			   std::unique_ptr<Material> material)
	  : m_name(name)
	  , m_attributes(std::move(attributes))
	  , m_zippedAttributes(MeshPrivate::zipAttributes(m_attributes))
	  , m_indices(std::move(indices))
	  , m_material(std::move(material))
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