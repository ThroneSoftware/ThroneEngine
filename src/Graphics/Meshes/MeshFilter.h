#pragma once

#include "../Models/Model.h"
#include "Mesh.h"

namespace trg
{
	struct MeshFilter
	{
		const Mesh& m_mesh;
		const Model& m_model;
	};
}  // namespace trg