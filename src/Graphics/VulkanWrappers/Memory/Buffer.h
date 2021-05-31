#pragma once

#include "VmaUnique.h"

#include <Vendors/VulkanMemoryAllocator/vk_mem_alloc.hpp>
#include <Vulkan/vulkan.hpp>

namespace trg::vkwrappers
{
	class Buffer
	{
	public:
		using VkHandleType = vk::Buffer;

	public:
		Buffer(vk::DeviceSize bufferSize, vk::BufferUsageFlagBits bufferUsage, vma::MemoryUsage memoryUsage);
		virtual ~Buffer() = default;

		void updateWithHostMemory(vk::DeviceSize dataSize, void* srcData);

	protected:
		VkHandleType& getVkHandle();
		const VkHandleType& getVkHandle() const;

	private:
		VmaUnique<vk::Buffer> m_buffer;
	};
}  // namespace trg::vkwrappers