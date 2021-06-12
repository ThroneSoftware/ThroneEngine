#include "Material.h"

namespace trg
{
	Material::Material(vk::Device& device, MaterialInfo& materialInfo)
	  : m_materialInfo(materialInfo)
	  , m_baseColorImage(
			vkwrappers::Image(device,
							  vk::ImageType::e2D,
							  vkwrappers::imageLayoutToVkFormat(m_materialInfo.m_baseColorTexture->getLayout()),
							  vk::Extent3D(m_materialInfo.m_baseColorTexture->getWidth(), m_materialInfo.m_baseColorTexture->getHeight()),
							  1 /*mipmapCount*/,
							  1 /*layerCount*/,
							  vk::SampleCountFlagBits::e1,
							  vk::ImageUsageFlagBits::eSampled,
							  vk::ImageLayout::eShaderReadOnlyOptimal))
	  , m_baseColorTexture(vkwrappers::ImageSampler(device, m_baseColorImage, vk::ShaderStageFlagBits::eAllGraphics))
	  , m_descriptorSet(device, getDescriptors(), static_cast<uint32_t>(vkwrappers::StandardDescriptorSetLocations::Material))
	{
		m_baseColorImage.updateWithHostMemory(m_materialInfo.m_baseColorTexture->getData().size_bytes(),
											  m_materialInfo.m_baseColorTexture->getData().data());
	}

	void Material::bind(vkwrappers::BindableBindInfo& bindInfo)
	{
		m_descriptorSet.bind(bindInfo);
	}

	std::span<const vkwrappers::Descriptor> Material::getDescriptors() const
	{
		return std::span(&m_baseColorTexture.getDescriptor(), 1);
	}

	const MaterialInfo& Material::getMaterialInfo() const
	{
		return m_materialInfo;
	}
}  // namespace trg