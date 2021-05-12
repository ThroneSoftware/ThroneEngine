#include "Descriptor.h"

#include <Utilities/Overload.h>

namespace trg
{
	Descriptor::Descriptor(DescriptorInfo descriptorInfo, vk::DescriptorType descriptorType, vk::ShaderStageFlagBits shaderStage)
	  : m_descriptorInfo(std::move(descriptorInfo))
	  , m_descriptorType(descriptorType)
	  , m_shaderStage(shaderStage)
	{
	}

	vk::WriteDescriptorSet Descriptor::getWriteDescriptorSet(vk::DescriptorSet& descriptorSet, uint32_t binding)
	{
		constexpr auto dstArrayElement = 0;
		// The real value for descriptorCount is set by the std::visit
		constexpr auto descriptorCount = 0;
		auto writeDescriptorSet = vk::WriteDescriptorSet(descriptorSet, binding, dstArrayElement, descriptorCount, m_descriptorType);

		std::visit(tru::Overload(
					   [&writeDescriptorSet](vk::DescriptorImageInfo& info) {
						   writeDescriptorSet.setImageInfo(info);
					   },
					   [&writeDescriptorSet](std::vector<vk::DescriptorImageInfo>& infos) {
						   writeDescriptorSet.setImageInfo(infos);
					   },
					   [&writeDescriptorSet](vk::DescriptorBufferInfo& info) {
						   writeDescriptorSet.setBufferInfo(info);
					   },
					   [&writeDescriptorSet](TexelBufferView& view) {
						   writeDescriptorSet.setTexelBufferView(view);
					   }),
				   m_descriptorInfo);

		return writeDescriptorSet;
	}
}  // namespace trg
