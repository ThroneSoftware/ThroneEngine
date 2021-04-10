#include "Buffer.h"

#include "VmaAllocator.h"

namespace trg
{
	namespace BufferPrivate
	{
		auto makeBuffer(vk::DeviceSize bufferSize, vk::BufferUsageFlagBits bufferUsage)
		{
			vk::BufferCreateInfo bufferInfo = vk::BufferCreateInfo({}, bufferSize, bufferUsage);

			vma::AllocationCreateInfo allocationCreateInfo = vma::AllocationCreateInfo();
			allocationCreateInfo.usage = vma::MemoryUsage::eGpuOnly;

			return g_vmaDefaultAllocator.createBuffer(bufferInfo, allocationCreateInfo);
		}
	}  // namespace BufferPrivate

	Buffer::Buffer(vk::DeviceSize bufferSize, vk::BufferUsageFlagBits bufferUsage)
	  : m_buffer(BufferPrivate::makeBuffer(bufferSize, bufferUsage))
	{
	}
}  // namespace trg