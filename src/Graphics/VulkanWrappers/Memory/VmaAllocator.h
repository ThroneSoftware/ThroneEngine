#pragma once

#include <Utilities/MemoryRegion.h>

#include <Vendors/VulkanMemoryAllocator/vk_mem_alloc.hpp>

namespace trg::vkwrappers
{
	extern vma::Allocator g_vmaDefaultAllocator;

	void initializeVmaDefaultAllocator(vk::PhysicalDevice& physicalDevice, vk::Device& device);

	void allocateHostMemory(tru::MemoryRegion memory, vma::Allocation& allocation);
}  // namespace trg::vkwrappers