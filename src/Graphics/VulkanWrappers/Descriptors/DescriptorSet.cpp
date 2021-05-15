#include "DescriptorSet.h"


namespace trg
{
	namespace DescriptorSetPrivate
	{
		auto makeDescriptorPool(vk::Device& device, std::span<std::reference_wrapper<const Descriptor>> descriptors)
		{
			std::vector<vk::DescriptorPoolSize> descriptorPoolSizes;
			descriptorPoolSizes.reserve(descriptors.size());

			for(const auto& descriptor: descriptors)
			{
				descriptorPoolSizes.emplace_back(descriptor.get().getDescriptorType(), descriptor.get().getDescriptorCount());
			}

			auto descriptorPoolCreateInfo = vk::DescriptorPoolCreateInfo({}, 1 /*maxSets*/, descriptorPoolSizes);

			return device.createDescriptorPoolUnique(descriptorPoolCreateInfo);
		}

		auto makeDescriptorSet(vk::Device& device, vk::DescriptorPool& pool, DescriptorSetLayout& layout)
		{
			auto allocateInfo = vk::DescriptorSetAllocateInfo(pool, *layout);

			auto descriptorSets = device.allocateDescriptorSetsUnique(allocateInfo);
			assert(descriptorSets.size() == 1);

			return std::move(descriptorSets.front());
		}
	}  // namespace DescriptorSetPrivate

	DescriptorSet::DescriptorSet(vk::Device& device, std::span<std::reference_wrapper<const Descriptor>> descriptors, uint32_t setLocation)
	  : m_device(device)
	  , m_descriptorPool(DescriptorSetPrivate::makeDescriptorPool(device, descriptors))
	  , m_layout(device, descriptors)
	  , m_descriptorSet(DescriptorSetPrivate::makeDescriptorSet(device, *m_descriptorPool, m_layout))
	  , m_setLocation(setLocation)
	{
		update(descriptors);
	}

	DescriptorSet::VkHandleType& DescriptorSet::getVkHandle()
	{
		return m_descriptorSet.get();
	}

	const DescriptorSet::VkHandleType& DescriptorSet::getVkHandle() const
	{
		return m_descriptorSet.get();
	}

	DescriptorSet::VkHandleType& DescriptorSet::operator*()
	{
		return getVkHandle();
	}

	const DescriptorSet::VkHandleType& DescriptorSet::operator*() const
	{
		return getVkHandle();
	}

	DescriptorSet::VkHandleType* DescriptorSet::operator->()
	{
		return &getVkHandle();
	}

	const DescriptorSet::VkHandleType* DescriptorSet::operator->() const
	{
		return &getVkHandle();
	}

	void DescriptorSet::update(std::span<std::reference_wrapper<const Descriptor>> descriptors)
	{
		std::vector<vk::WriteDescriptorSet> writeDescriptorSets;
		writeDescriptorSets.reserve(descriptors.size());

		for(size_t i = 0; i < descriptors.size(); ++i)
		{
			writeDescriptorSets.emplace_back(descriptors[i].get().getWriteDescriptorSet(*m_descriptorSet, i));
		}

		m_device.updateDescriptorSets(writeDescriptorSets, {});
	}
}  // namespace trg