#pragma once

#include "../Memory/Image.h"
#include "Descriptor.h"

namespace trg::vkwrappers
{
	class ImageSampler
	{
	public:
		using VkHandleType = vk::Sampler;

	public:
		ImageSampler(vk::Device& device, const Image& image, vk::ShaderStageFlagBits shaderStage);

		VkHandleType& getVkHandle();
		const VkHandleType& getVkHandle() const;

		VkHandleType& operator*();
		const VkHandleType& operator*() const;

		VkHandleType* operator->();
		const VkHandleType* operator->() const;

		const Descriptor& getDescriptor() const;

	private:
		vk::UniqueSampler m_sampler;

		const Image& m_image;

		Descriptor m_descriptor;
	};
}  // namespace trg::vkwrappers