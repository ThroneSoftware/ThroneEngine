#include "IndexBuffer.h"

namespace trg::vkwrappers
{
	IndexBuffer::IndexBuffer(vk::DeviceSize bufferSize, vk::BufferUsageFlagBits bufferUsage, vma::MemoryUsage memoryUsage)
	  : Buffer(bufferSize, bufferUsage, memoryUsage)
	{
	}

	void IndexBuffer::bind(BindableBindInfo& bindInfo)
	{
		vk::DeviceSize offset = {0};
		bindInfo.commandBuffer->bindIndexBuffer(getVkHandle(), offset, vk::IndexType::eUint16);
	}
}  // namespace trg::vkwrappers
