#pragma once

#include "../VulkanWrappers/Descriptors/ImageSampler.h"
#include "MaterialInfo.h"

namespace trg
{
	class Material
	{
	public:
		Material(vk::Device& device, MaterialInfo&& materialInfo);

	private:
		MaterialInfo m_materialInfo;

		vkwrappers::Image m_baseColorImage;
		vkwrappers::ImageSampler m_baseColorTexture;
	};
}  // namespace trg