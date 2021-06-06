#include "Material.h"

namespace trg
{
	Material::Material(vk::Device& device, MaterialInfo&& materialInfo)
	  : m_materialInfo(std::move(materialInfo))
	  , m_baseColorImage(
			vkwrappers::Image(device,
							  vk::ImageType::e2D,
							  vk::Format::eR8G8B8A8Unorm,
							  vk::Extent3D(m_materialInfo.m_baseColorTexture->getWidth(), m_materialInfo.m_baseColorTexture->getHeight()),
							  1 /*mipmapCount*/,
							  1 /*layerCount*/,
							  vk::SampleCountFlagBits::e1,
							  vk::ImageUsageFlagBits::eSampled,
							  vk::ImageLayout::eShaderReadOnlyOptimal))
	  , m_baseColorTexture(vkwrappers::ImageSampler(device, m_baseColorImage, vk::ShaderStageFlagBits::eAllGraphics))
	{
		m_baseColorImage.updateWithHostMemory(m_materialInfo.m_baseColorTexture->getData().size() * sizeof(uint8_t),
											  m_materialInfo.m_baseColorTexture->getData().data());
	}
}  // namespace trg