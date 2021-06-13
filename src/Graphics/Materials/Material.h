#pragma once

#include "../VulkanWrappers/Bindable/Bindable.h"
#include "../VulkanWrappers/Commands/CommandQueue.h"
#include "../VulkanWrappers/Descriptors/DescriptorSet.h"
#include "../VulkanWrappers/Descriptors/ImageSampler.h"
#include "MaterialInfo.h"

namespace trg
{
	class Material
	{
	public:
		Material(vk::Device& device, vkwrappers::CommandQueue& commandQueue, MaterialInfo& materialInfo);

		void bind(vkwrappers::BindableBindInfo& bindInfo);

		const MaterialInfo& getMaterialInfo() const;

		const vkwrappers::DescriptorSet& getDescriptorSet() const;

	private:
		std::span<const vkwrappers::Descriptor> getDescriptors() const;

		// Do not change the order of the members because of the member initializer list.
		MaterialInfo& m_materialInfo;

		vkwrappers::Image m_baseColorImage;
		vkwrappers::ImageSampler m_baseColorTexture;

		vkwrappers::DescriptorSet m_descriptorSet;
	};
}  // namespace trg