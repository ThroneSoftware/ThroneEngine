#pragma once

#include "Mesh.h"

#include <string>
#include <vector>

namespace trg
{
	class Model
	{
	public:
		Model(const std::string& name);

		void addMesh(Mesh&& mesh);

	private:
		std::string m_name;

		std::vector<Mesh> m_meshes;
	};
}  // namespace trg