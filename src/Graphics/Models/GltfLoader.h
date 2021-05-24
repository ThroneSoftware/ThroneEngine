#pragma once

#include <filesystem>

namespace trg
{
	class GltfLoader
	{
	public:
		GltfLoader();

		void loadFromFile(const std::filesystem::path& path);
	private:
	};
}