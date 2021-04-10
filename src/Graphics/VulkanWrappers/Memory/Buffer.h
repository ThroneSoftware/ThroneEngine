#pragma once

#include "VmaUnique.h"

#include <Vendors/VulkanMemoryAllocator/vk_mem_alloc.hpp>
#include <Vulkan/vulkan.hpp>

namespace trg
{
	class Buffer
	{
	public:
		Buffer(vk::DeviceSize bufferSize, vk::BufferUsageFlagBits bufferUsage);

	private:
		VmaUnique<vk::Buffer> m_buffer;
	};
}  // namespace trg