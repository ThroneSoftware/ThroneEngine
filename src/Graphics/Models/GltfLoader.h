#pragma once

#include "ModelLoader.h"

#include <filesystem>

namespace trg
{
	class GltfLoader : public ModelLoader
	{
	public:
		GltfLoader();

		Model loadFromFile(const std::filesystem::path& path) override;

	private:
	};
}  // namespace trg