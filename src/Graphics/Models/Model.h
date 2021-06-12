#pragma once

#include "../Materials/Material.h"
#include "../Meshes/Mesh.h"

#include <string>
#include <vector>

namespace trg
{
	class Model
	{
	public:
		Model(const std::string& name);

		void setMaterials(std::vector<std::unique_ptr<MaterialInfo>> materialInfos);
		void addMesh(Mesh&& mesh);

	private:
		std::string m_name;

		std::vector<std::unique_ptr<MaterialInfo>> m_materialInfos;
		std::vector<Mesh> m_meshes;
	};
}  // namespace trg