#include "DescriptorSetLayout.h"

namespace trg::vkwrappers
{
	namespace DescriptorSetLayoutPrivate
	{
		auto makeDescriptorSetLayout(vk::Device& device, std::span<const Descriptor> descriptors)
		{
			std::vector<vk::DescriptorSetLayoutBinding> bindings;
			bindings.reserve(descriptors.size());
			for(uint32_t i = 0; i < descriptors.size(); ++i)
			{
				bindings.emplace_back(descriptors[i].getDescriptorSetLayoutBinding(i));
			}

			auto createInfo = vk::DescriptorSetLayoutCreateInfo({}, bindings);
			return device.createDescriptorSetLayoutUnique(createInfo);
		}
	}  // namespace DescriptorSetLayoutPrivate

	DescriptorSetLayout::DescriptorSetLayout(vk::Device& device, std::span<const Descriptor> descriptors)
	  : m_descriptorSetLayout(DescriptorSetLayoutPrivate::makeDescriptorSetLayout(device, descriptors))
	{
	}

	DescriptorSetLayout::VkHandleType& DescriptorSetLayout::getVkHandle()
	{
		return m_descriptorSetLayout.get();
	}

	const DescriptorSetLayout::VkHandleType& DescriptorSetLayout::getVkHandle() const
	{
		return m_descriptorSetLayout.get();
	}

	DescriptorSetLayout::VkHandleType& DescriptorSetLayout::operator*()
	{
		return getVkHandle();
	}

	const DescriptorSetLayout::VkHandleType& DescriptorSetLayout::operator*() const
	{
		return getVkHandle();
	}

	DescriptorSetLayout::VkHandleType* DescriptorSetLayout::operator->()
	{
		return &getVkHandle();
	}

	const DescriptorSetLayout::VkHandleType* DescriptorSetLayout::operator->() const
	{
		return &getVkHandle();
	}
}  // namespace trg::vkwrappers
