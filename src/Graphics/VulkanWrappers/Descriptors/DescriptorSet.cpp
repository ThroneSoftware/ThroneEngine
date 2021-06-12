#include "DescriptorSet.h"

#include <gsl/gsl>

namespace trg::vkwrappers
{
	namespace DescriptorSetPrivate
	{
		auto makeDescriptorPool(vk::Device& device, std::span<const Descriptor> descriptors)
		{
			std::vector<vk::DescriptorPoolSize> descriptorPoolSizes;
			descriptorPoolSizes.reserve(descriptors.size());

			for(const auto& descriptor: descriptors)
			{
				descriptorPoolSizes.emplace_back(descriptor.getDescriptorType(), descriptor.getDescriptorCount());
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

	DescriptorSet::DescriptorSet(vk::Device& device, std::span<const Descriptor> descriptors, uint32_t setLocation)
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

	void DescriptorSet::update(std::span<const Descriptor> descriptors)
	{
		std::vector<vk::WriteDescriptorSet> writeDescriptorSets;
		writeDescriptorSets.reserve(descriptors.size());

		for(size_t i = 0; i < descriptors.size(); ++i)
		{
			writeDescriptorSets.emplace_back(descriptors[i].getWriteDescriptorSet(*m_descriptorSet, gsl::narrow<uint32_t>(i)));
		}

		m_device.updateDescriptorSets(writeDescriptorSets, {});
	}

	void DescriptorSet::bind(vkwrappers::BindableBindInfo& bindInfo)
	{
		bindInfo.m_commandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
													 bindInfo.m_pipelineLayout,
													 m_setLocation,
													 *m_descriptorSet,
													 {});
	}

	const DescriptorSetLayout& DescriptorSet::getLayout() const
	{
		return m_layout;
	}
}  // namespace trg::vkwrappers