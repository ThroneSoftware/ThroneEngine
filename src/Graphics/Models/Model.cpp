#include "Model.h"
namespace trg
{
	Model::Model(const std::string& name)
	  : m_name(name)
	{
	}

	void Model::setMaterials(std::vector<std::unique_ptr<MaterialInfo>> materialInfos)
	{
		m_materialInfos = std::move(materialInfos);
	}

	void Model::addMesh(Mesh&& mesh)
	{
		m_meshes.emplace_back(std::move(mesh));
	}
}  // namespace trg