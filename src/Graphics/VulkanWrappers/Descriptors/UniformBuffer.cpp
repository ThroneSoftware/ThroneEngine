#include "UniformBuffer.h"

namespace trg::vkwrappers
{
	UniformBuffer::UniformBuffer(vk::DeviceSize bufferSize,
								 vk::BufferUsageFlagBits bufferUsage,
								 vma::MemoryUsage memoryUsage,
								 vk::ShaderStageFlags shaderStage)
	  : Buffer(bufferSize, bufferUsage, memoryUsage)
	  , m_descriptor(vk::DescriptorBufferInfo(getVkHandle(), 0 /*offset*/, VK_WHOLE_SIZE /*range*/),
					 vk::DescriptorType::eUniformBuffer,
					 shaderStage)
	{
	}

	const Descriptor& UniformBuffer::getDescriptor() const
	{
		return m_descriptor;
	}
}  // namespace trg::vkwrappers