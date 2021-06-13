#include "Material.h"

namespace trg
{
	namespace MaterialPrivate
	{
		vkwrappers::Image makeImage(vk::Device& device, MaterialInfo& materialInfo)
		{
			auto imageFormat = vkwrappers::imageLayoutToVkFormat(materialInfo.m_baseColorTexture->getLayout());

			auto image = vkwrappers::Image(
				device,
				vk::ImageType::e2D,
				imageFormat,
				vk::Extent3D(materialInfo.m_baseColorTexture->getWidth(), materialInfo.m_baseColorTexture->getHeight(), 1 /*depth*/),
				1 /*mipmapCount*/,
				1 /*layerCount*/,
				vk::SampleCountFlagBits::e1,
				vk::ImageTiling::eLinear,
				vk::ImageUsageFlagBits::eSampled,
				vk::ImageLayout::eUndefined,
				vma::MemoryUsage::eCpuToGpu);

			image.addImageView(vk::ImageAspectFlagBits::eColor, vk::ImageViewType::e2D, imageFormat, 0 /*layer*/, 1 /*layerCount*/);

			return image;
		}
	}  // namespace MaterialPrivate

	Material::Material(vk::Device& device, MaterialInfo& materialInfo)
	  : m_materialInfo(materialInfo)
	  , m_baseColorImage(MaterialPrivate::makeImage(device, m_materialInfo))
	  , m_baseColorTexture(vkwrappers::ImageSampler(device, m_baseColorImage, vk::ShaderStageFlagBits::eAllGraphics))
	  //, m_descriptorSet(device, getDescriptors(), static_cast<uint32_t>(vkwrappers::StandardDescriptorSetLocations::Material))
	  , m_descriptorSet(device, getDescriptors(), static_cast<uint32_t>(0))
	{
		m_baseColorImage.updateWithHostMemory(m_materialInfo.m_baseColorTexture->getData());
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

	const vkwrappers::DescriptorSet& Material::getDescriptorSet() const
	{
		return m_descriptorSet;
	}
}  // namespace trg