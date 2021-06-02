#include <Graphics/VulkanWrappers/Shaders/Shader.h>
#include <catch.hpp>

namespace Tests
{
	SCENARIO("Test readShaderCode")
	{
		std::filesystem::current_path(std::filesystem::path(__FILE__).parent_path());

		REQUIRE_THROWS(trg::vkwrappers::readShaderCode("./TestData/invalid"));
		REQUIRE_THROWS(trg::vkwrappers::readShaderCode("./TestData"));
		REQUIRE_THROWS(trg::vkwrappers::readShaderCode("./TestData/File.txt"));
		REQUIRE_THROWS(trg::vkwrappers::readShaderCode("./TestData/File.spv"));

		REQUIRE_NOTHROW(trg::vkwrappers::readShaderCode("./TestData/CompiledShader.spv"));

		auto shaderCode = trg::vkwrappers::readShaderCode("./TestData/CompiledShader.spv");
		REQUIRE(shaderCode.size() > 0);
	}
}  // namespace Tests