#pragma once

#include "../Materials/Material.h"
#include "../Meshes/Mesh.h"

#include <span>
#include <string>
#include <vector>

namespace trg
{
	class Model
	{
	public:
		Model(const std::string& name);

		void setMaterials(std::vector<MaterialInfo> materialInfos);
		void addMesh(Mesh&& mesh);

		std::span<MaterialInfo> getMaterials();
		std::span<Mesh> getMeshes();

	private:
		std::string m_name;

		std::vector<MaterialInfo> m_materialInfos;
		std::vector<Mesh> m_meshes;
	};
}  // namespace trg