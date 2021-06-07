#pragma once

#include "Descriptor.h"

#include <Vulkan/vulkan.hpp>

namespace trg::vkwrappers
{
	class DescriptorSetLayout
	{
	public:
		using VkHandleType = vk::DescriptorSetLayout;

	public:
		DescriptorSetLayout(vk::Device& device, std::span<const Descriptor> descriptors);

		VkHandleType& getVkHandle();
		const VkHandleType& getVkHandle() const;

		VkHandleType& operator*();
		const VkHandleType& operator*() const;

		VkHandleType* operator->();
		const VkHandleType* operator->() const;

	private:
		vk::UniqueDescriptorSetLayout m_descriptorSetLayout;
	};
}  // namespace trg::vkwrappers