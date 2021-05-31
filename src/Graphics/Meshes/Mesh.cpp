#include "Mesh.h"
namespace trg
{
	Mesh::Mesh(const std::string& name, std::vector<MeshAttribute> attributes, std::vector<uint16_t> indices)
	  : m_name(name)
	  , m_attributes(std::move(attributes))
	  , m_indices(std::move(indices))
	{
	}
}  // namespace trg