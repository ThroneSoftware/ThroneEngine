#include <Graphics/VulkanWrappers/Shaders/Shader.h>
#include <catch.hpp>

namespace Tests
{
	SCENARIO("Test readShaderCode")
	{
		std::filesystem::current_path(std::filesystem::path(__FILE__).parent_path());

		REQUIRE_THROWS(trg::readShaderCode("./TestData/invalid"));
		REQUIRE_THROWS(trg::readShaderCode("./TestData"));
		REQUIRE_THROWS(trg::readShaderCode("./TestData/File.txt"));
		REQUIRE_THROWS(trg::readShaderCode("./TestData/File.spv"));

		REQUIRE_NOTHROW(trg::readShaderCode("./TestData/CompiledShader.spv"));

		auto shaderCode = trg::readShaderCode("./TestData/CompiledShader.spv");
		REQUIRE(shaderCode.size() > 0);
	}
}  // namespace Tests