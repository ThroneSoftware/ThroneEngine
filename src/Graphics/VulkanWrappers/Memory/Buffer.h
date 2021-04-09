#pragma once

#include <Vendors/VulkanMemoryAllocator/vk_mem_alloc.hpp>
#include <Vulkan/vulkan.hpp>

namespace trg
{
	class Buffer
	{
	public:
		Buffer(vk::DeviceSize bufferSize, vk::BufferUsageFlagBits bufferUsage);

		Buffer(const Buffer& other) = delete;
		Buffer& operator=(const Buffer& other) = delete;

		Buffer(Buffer&& other) = delete;
		Buffer& operator=(Buffer&& other) = delete;

		~Buffer();

	private:
		Buffer(std::pair<vk::Buffer, vma::Allocation>&& buffer);

		vk::Buffer m_buffer;
		vma::Allocation m_allocation;
	};
}  // namespace trg