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

			return vmaDefaultAllocator.createBuffer(bufferInfo, allocationCreateInfo);
		}
	}  // namespace BufferPrivate

	Buffer::Buffer(vk::DeviceSize bufferSize, vk::BufferUsageFlagBits bufferUsage)
	  : Buffer(BufferPrivate::makeBuffer(bufferSize, bufferUsage))
	{
	}

	Buffer::~Buffer()
	{
		vmaDefaultAllocator.destroyBuffer(m_buffer, m_allocation);
	}

	Buffer::Buffer(std::pair<vk::Buffer, vma::Allocation>&& buffer)
	  : m_buffer(std::move(buffer.first))
	  , m_allocation(buffer.second)
	{
	}
}  // namespace trg