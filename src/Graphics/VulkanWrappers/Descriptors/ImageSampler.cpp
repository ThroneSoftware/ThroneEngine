#include "ImageSampler.h"

namespace trg::vkwrappers
{
	namespace ImageSamplerPrivate
	{
		auto makeSampler(vk::Device& device)
		{
			auto createInfo = vk::SamplerCreateInfo({},
													vk::Filter::eLinear,
													vk::Filter::eLinear,
													vk::SamplerMipmapMode::eLinear,
													vk::SamplerAddressMode::eRepeat,
													vk::SamplerAddressMode::eRepeat,
													vk::SamplerAddressMode::eRepeat,
													0.0f /*mipLodBias_*/,
													false /*anisotropyEnable_*/,
													16.0f /*maxAnisotropy_*/,
													false /*compareEnable_*/,
													vk::CompareOp::eNever,
													0.0f /*minLod_*/,
													1.0f /*maxLod_*/,
													vk::BorderColor::eFloatOpaqueBlack,
													false /*unnormalizedCoordinates_*/);

			return device.createSamplerUnique(createInfo);
		}
	}  // namespace ImageSamplerPrivate

	ImageSampler::ImageSampler(vk::Device& device, const Image& image, vk::ShaderStageFlagBits shaderStage)
	  : m_sampler(ImageSamplerPrivate::makeSampler(device))
	  , m_image(image)
	  , m_descriptor(vk::DescriptorImageInfo(*m_sampler, *m_image.getImageView(), m_image.getImageLayout()),
					 vk::DescriptorType::eCombinedImageSampler,
					 shaderStage)
	{
	}

	ImageSampler::VkHandleType& ImageSampler::getVkHandle()
	{
		return m_sampler.get();
	}

	const ImageSampler::VkHandleType& ImageSampler::getVkHandle() const
	{
		return m_sampler.get();
	}

	ImageSampler::VkHandleType& ImageSampler::operator*()
	{
		return getVkHandle();
	}

	const ImageSampler::VkHandleType& ImageSampler::operator*() const
	{
		return getVkHandle();
	}

	ImageSampler::VkHandleType* ImageSampler::operator->()
	{
		return &getVkHandle();
	}

	const ImageSampler::VkHandleType* ImageSampler::operator->() const
	{
		return &getVkHandle();
	}
}  // namespace trg::vkwrappers