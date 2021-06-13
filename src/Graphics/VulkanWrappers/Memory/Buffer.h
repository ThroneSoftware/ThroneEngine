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

		Buffer(const Buffer& other) = delete;
		Buffer& operator=(const Buffer& other) = delete;

		Buffer(Buffer&& other) = default;
		Buffer& operator=(Buffer&& other) = default;

		virtual ~Buffer() = default;

		VkHandleType& getVkHandle();
		const VkHandleType& getVkHandle() const;

		VkHandleType& operator*();
		const VkHandleType& operator*() const;

		VkHandleType* operator->();
		const VkHandleType* operator->() const;

		void updateWithHostMemory(tru::MemoryRegion memory);

	private:
		VmaUnique<vk::Buffer> m_buffer;
	};
}  // namespace trg::vkwrappers