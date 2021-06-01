#include <Graphics/Models/GltfLoader.h>

#include <catch.hpp>

#include <filesystem>

namespace Tests
{
	SCENARIO("Test GltfLoader")
	{
		std::filesystem::current_path(std::filesystem::path(__FILE__).parent_path());

		trg::GltfLoader loader;
		auto cubeModel = loader.loadFromFile("./TestData/cube.gltf");

		auto voyagerModel = loader.loadFromFile("./TestData/voyager.gltf");
	}
}  // namespace Tests