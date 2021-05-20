#include "Shader.h"

#include <fmt/format.h>

#include <fstream>

namespace trg
{
	namespace ShaderPrivate
	{
		auto makeShaderModule(vk::Device& device, std::span<const std::byte> shaderCode)
		{
			// The spirv shaders are read as 8 bits, but vulkan expect the pointer to be of 32 bits for some reason
			const uint32_t* shaderCodePtr = reinterpret_cast<const uint32_t*>(shaderCode.data());

			auto createInfo = vk::ShaderModuleCreateInfo({}, shaderCode.size(), shaderCodePtr);

			return device.createShaderModuleUnique(createInfo);
		}
	}  // namespace ShaderPrivate

	std::vector<std::byte> readShaderCode(const std::filesystem::path& shaderPath)
	{
		if(shaderPath.has_filename() && shaderPath.has_extension() && shaderPath.extension() == ".spv")
		{
			if(std::filesystem::exists(shaderPath))
			{
				auto ifstream = std::basic_ifstream<std::byte>(shaderPath, std::ios::binary);

				auto bufferIterator = std::istreambuf_iterator(ifstream);

				return std::vector<std::byte>(bufferIterator, {});
			}
			else
			{
				throw std::runtime_error(fmt::format("File does not exist. Path: {}", shaderPath.string()));
			}
		}
		else
		{
			throw std::runtime_error(fmt::format("Path is not a .spv file. Path: {}", shaderPath.string()));
		}
	}

	Shader::Shader(vk::Device& device,
				   std::span<const std::byte> shaderCode,
				   vk::ShaderStageFlagBits shaderStage,
				   const std::string& entryPoint)
	  : m_shaderModule(ShaderPrivate::makeShaderModule(device, shaderCode))
	  , m_shaderStage(shaderStage)
	  , m_entryPoint(entryPoint)
	{
	}

	Shader::Shader(vk::Device& device,
				   const std::filesystem::path& shaderPath,
				   vk::ShaderStageFlagBits shaderStage,
				   const std::string& entryPoint)
	  : Shader(device, readShaderCode(shaderPath), shaderStage, entryPoint)
	{
	}

	vk::PipelineShaderStageCreateInfo Shader::getShaderStageCreateInfo() const
	{
		return vk::PipelineShaderStageCreateInfo({}, m_shaderStage, *m_shaderModule, m_entryPoint.c_str(), nullptr);
	}
}  // namespace trg