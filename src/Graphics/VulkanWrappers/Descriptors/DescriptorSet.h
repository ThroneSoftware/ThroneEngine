#pragma once

#include "Descriptor.h"
#include "DescriptorSetLayout.h"

#include <Vulkan/vulkan.hpp>

namespace trg::vkwrappers
{
	class DescriptorSet
	{
	public:
		using VkHandleType = vk::DescriptorSet;

	public:
		DescriptorSet(vk::Device& device, std::span<std::reference_wrapper<const Descriptor>> descriptors, uint32_t setLocation);

		VkHandleType& getVkHandle();
		const VkHandleType& getVkHandle() const;

		VkHandleType& operator*();
		const VkHandleType& operator*() const;

		VkHandleType* operator->();
		const VkHandleType* operator->() const;

		void update(std::span<std::reference_wrapper<const Descriptor>> descriptors);

	private:
		vk::Device& m_device;
		vk::UniqueDescriptorPool m_descriptorPool;
		DescriptorSetLayout m_layout;
		vk::UniqueDescriptorSet m_descriptorSet;

		uint32_t m_setLocation;
	};
}  // namespace trg::vkwrappers