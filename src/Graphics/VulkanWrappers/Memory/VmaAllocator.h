#pragma once

#include <Vendors/VulkanMemoryAllocator/vk_mem_alloc.hpp>

namespace trg
{
	extern vma::Allocator vmaDefaultAllocator;

	void initializeVmaDefaultAllocator(vk::PhysicalDevice& physicalDevice, vk::Device& device);
}  // namespace trg