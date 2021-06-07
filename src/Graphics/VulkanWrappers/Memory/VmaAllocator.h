#pragma once

#include <Vendors/VulkanMemoryAllocator/vk_mem_alloc.hpp>

namespace trg::vkwrappers
{
	extern vma::Allocator g_vmaDefaultAllocator;

	void initializeVmaDefaultAllocator(vk::PhysicalDevice& physicalDevice, vk::Device& device);

	void allocateHostMemory(vk::DeviceSize dataSize, const void* srcData, vma::Allocation& allocation);
}  // namespace trg::vkwrappers