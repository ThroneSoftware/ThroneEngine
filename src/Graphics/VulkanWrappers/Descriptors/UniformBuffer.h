#pragma once

#include "../Memory/Buffer.h"
#include "Descriptor.h"

namespace trg::vkwrappers
{
	class UniformBuffer : public Buffer
	{
	public:
		UniformBuffer(vk::DeviceSize bufferSize,
					  vk::BufferUsageFlagBits bufferUsage,
					  vma::MemoryUsage memoryUsage,
					  vk::ShaderStageFlags shaderStage);

		const Descriptor& getDescriptor() const;

	private:
		Descriptor m_descriptor;
	};
}  // namespace trg::vkwrappers