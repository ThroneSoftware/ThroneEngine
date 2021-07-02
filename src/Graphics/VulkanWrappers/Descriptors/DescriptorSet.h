#pragma once

#include "../Bindable/Bindable.h"
#include "Descriptor.h"
#include "DescriptorSetLayout.h"

#include <Vulkan/vulkan.hpp>

namespace trg::vkwrappers
{
	enum class StandardDescriptorSetLocations : uint32_t
	{
		// Reserved range : [0, 7], [100 000, 4 294 967 295]
		// User range : [8, 99 999]

		Global = 0,
		Scene = 1,
		Material = 2,
		Entity = 3,

		NonStandard = std::numeric_limits<uint32_t>::max()
	};

	class DescriptorSet
	{
	public:
		using VkHandleType = vk::DescriptorSet;

	public:
		DescriptorSet(vk::Device& device, std::span<const Descriptor> descriptors, uint32_t setLocation);

		VkHandleType& getVkHandle();
		const VkHandleType& getVkHandle() const;

		VkHandleType& operator*();
		const VkHandleType& operator*() const;

		VkHandleType* operator->();
		const VkHandleType* operator->() const;

		void update(std::span<const Descriptor> descriptors);

		void bind(vkwrappers::BindableBindInfo& bindInfo);

		const DescriptorSetLayout& getLayout() const;

	private:
		vk::Device& m_device;
		vk::UniqueDescriptorPool m_descriptorPool;
		DescriptorSetLayout m_layout;
		vk::UniqueDescriptorSet m_descriptorSet;

		uint32_t m_setLocation;
	};
}  // namespace trg::vkwrappers