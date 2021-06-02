#pragma once

#include "Model.h"

#include <filesystem>

namespace trg
{
	class ModelLoader
	{
	public:
		virtual ~ModelLoader() = default;

		virtual Model loadFromFile(const std::filesystem::path& path) = 0;
	};
}  // namespace trg