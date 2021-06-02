#include "Descriptor.h"

#include <Utilities/Overload.h>

#include <gsl/gsl>

namespace trg::vkwrappers
{
	namespace DescriptorPrivate
	{
		uint32_t getDescriptorCount(const DescriptorInfo& descriptorInfo)
		{
			return std::visit(tru::Overload(
								  [](const std::vector<vk::DescriptorImageInfo>& infos) {
									  return gsl::narrow<uint32_t>(infos.size());
								  },
								  [](const auto&) {
									  return uint32_t(1);
								  }),
							  descriptorInfo);
		}
	}  // namespace DescriptorPrivate


	Descriptor::Descriptor(DescriptorInfo descriptorInfo, vk::DescriptorType descriptorType, vk::ShaderStageFlagBits shaderStage)
	  : m_descriptorInfo(std::move(descriptorInfo))
	  , m_descriptorType(descriptorType)
	  , m_shaderStage(shaderStage)
	  , m_descriptorCount(DescriptorPrivate::getDescriptorCount(m_descriptorInfo))
	{
	}

	vk::WriteDescriptorSet Descriptor::getWriteDescriptorSet(vk::DescriptorSet& descriptorSet, uint32_t binding) const
	{
		constexpr auto dstArrayElement = 0;
		auto writeDescriptorSet = vk::WriteDescriptorSet(descriptorSet, binding, dstArrayElement, m_descriptorCount, m_descriptorType);

		std::visit(tru::Overload(
					   [&writeDescriptorSet](const vk::DescriptorImageInfo& info) {
						   writeDescriptorSet.setImageInfo(info);
					   },
					   [&writeDescriptorSet](const std::vector<vk::DescriptorImageInfo>& infos) {
						   writeDescriptorSet.setImageInfo(infos);
					   },
					   [&writeDescriptorSet](const vk::DescriptorBufferInfo& info) {
						   writeDescriptorSet.setBufferInfo(info);
					   },
					   [&writeDescriptorSet](const TexelBufferView& view) {
						   writeDescriptorSet.setTexelBufferView(view);
					   }),
				   m_descriptorInfo);

		return writeDescriptorSet;
	}

	vk::DescriptorSetLayoutBinding Descriptor::getDescriptorSetLayoutBinding(uint32_t binding) const
	{
		return vk::DescriptorSetLayoutBinding(binding, m_descriptorType, m_descriptorCount, m_shaderStage);
	}

	vk::DescriptorType Descriptor::getDescriptorType() const
	{
		return m_descriptorType;
	}

	uint32_t Descriptor::getDescriptorCount() const
	{
		return m_descriptorCount;
	}
}  // namespace trg::vkwrappers
