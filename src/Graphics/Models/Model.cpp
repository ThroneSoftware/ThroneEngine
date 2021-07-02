#include "Model.h"
namespace trg
{
	Model::Model(const std::string& name)
	  : m_name(name)
	{
	}

	void Model::setMaterials(std::vector<MaterialInfo> materialInfos)
	{
		m_materialInfos = std::move(materialInfos);
	}

	void Model::addMesh(Mesh&& mesh)
	{
		m_meshes.emplace_back(std::move(mesh));
	}

	std::span<MaterialInfo> Model::getMaterials()
	{
		return m_materialInfos;
	}

	std::span<Mesh> Model::getMeshes()
	{
		return m_meshes;
	}
}  // namespace trg