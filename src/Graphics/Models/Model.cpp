#include "Model.h"
namespace trg
{
	Model::Model(const std::string& name)
	  : m_name(name)
	{
	}

	void Model::addMesh(Mesh&& mesh)
	{
		m_meshes.emplace_back(std::move(mesh));
	}
}  // namespace trg