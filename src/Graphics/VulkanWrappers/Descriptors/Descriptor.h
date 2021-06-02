#pragma once

#include <Vulkan/vulkan.hpp>

#include <variant>

namespace trg::vkwrappers
{
	using TexelBufferView = vk::BufferView;
	using DescriptorInfo =
		std::variant<vk::DescriptorImageInfo, std::vector<vk::DescriptorImageInfo>, vk::DescriptorBufferInfo, TexelBufferView>;

	class Descriptor
	{
	public:
		Descriptor(DescriptorInfo descriptorInfo, vk::DescriptorType descriptorType, vk::ShaderStageFlagBits shaderStage);

		vk::WriteDescriptorSet getWriteDescriptorSet(vk::DescriptorSet& descriptorSet, uint32_t binding) const;
		vk::DescriptorSetLayoutBinding getDescriptorSetLayoutBinding(uint32_t binding) const;

		vk::DescriptorType getDescriptorType() const;
		uint32_t getDescriptorCount() const;

	private:
		DescriptorInfo m_descriptorInfo;
		vk::DescriptorType m_descriptorType;
		vk::ShaderStageFlagBits m_shaderStage;
		uint32_t m_descriptorCount;
	};
}  // namespace trg::vkwrappers