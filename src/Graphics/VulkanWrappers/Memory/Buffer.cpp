#include "Buffer.h"

#include "VmaAllocator.h"

namespace trg::vkwrappers
{
	namespace BufferPrivate
	{
		auto makeBuffer(vk::DeviceSize bufferSize, vk::BufferUsageFlagBits bufferUsage, vma::MemoryUsage memoryUsage)
		{
			vk::BufferCreateInfo bufferInfo = vk::BufferCreateInfo({}, bufferSize, bufferUsage);

			vma::AllocationCreateInfo allocationCreateInfo = vma::AllocationCreateInfo();
			allocationCreateInfo.usage = memoryUsage;

			return g_vmaDefaultAllocator.createBuffer(bufferInfo, allocationCreateInfo);
		}
	}  // namespace BufferPrivate

	Buffer::Buffer(vk::DeviceSize bufferSize, vk::BufferUsageFlagBits bufferUsage, vma::MemoryUsage memoryUsage)
	  : m_buffer(BufferPrivate::makeBuffer(bufferSize, bufferUsage, memoryUsage))
	{
	}

	void Buffer::updateWithHostMemory(vk::DeviceSize dataSize, void* srcData)
	{
		assert(dataSize > 0 && srcData != nullptr);

		void* dstData = nullptr;
		g_vmaDefaultAllocator.mapMemory(m_buffer.m_allocation, &dstData);
		assert(dstData != nullptr);

		memcpy(dstData, srcData, dataSize);

		g_vmaDefaultAllocator.unmapMemory(m_buffer.m_allocation);
	}

	Buffer::VkHandleType& Buffer::getVkHandle()
	{
		return m_buffer.m_value;
	}

	const Buffer::VkHandleType& Buffer::getVkHandle() const
	{
		return m_buffer.m_value;
	}
}  // namespace trg::vkwrappers