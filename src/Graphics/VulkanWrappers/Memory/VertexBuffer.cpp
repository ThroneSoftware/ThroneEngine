#include "VertexBuffer.h"

namespace trg
{
	VertexBuffer::VertexBuffer(vk::DeviceSize bufferSize,
							   vk::BufferUsageFlagBits bufferUsage,
							   vma::MemoryUsage memoryUsage,
							   uint32_t bindingIndex)
	  : Buffer(bufferSize, bufferUsage, memoryUsage)
	  , m_bindingIndex(bindingIndex)
	{
	}

	void VertexBuffer::bind(BindableBindInfo& bindInfo)
	{
		vk::DeviceSize offset = {0};
		bindInfo.commandBuffer->bindVertexBuffers(m_bindingIndex, getVkHandle(), offset);
	}
}  // namespace trg