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

		// No extension
		REQUIRE_THROWS(loader.loadFromFile("./TestData/invalid"));
		// Not a file
		REQUIRE_THROWS(loader.loadFromFile("./TestData"));
		// Wrong extension
		REQUIRE_THROWS(loader.loadFromFile("./TestData/model.t"));
		// File does not exists
		REQUIRE_THROWS(loader.loadFromFile("./TestData/model.gltf"));

		REQUIRE_NOTHROW(loader.loadFromFile("./TestData/cube.gltf"));
	}
}  // namespace Tests