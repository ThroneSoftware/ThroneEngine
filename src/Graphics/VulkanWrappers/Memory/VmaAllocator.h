#pragma once

#include <Vendors/VulkanMemoryAllocator/vk_mem_alloc.hpp>

namespace trg::vkwrappers
{
	extern vma::Allocator g_vmaDefaultAllocator;

	void initializeVmaDefaultAllocator(vk::PhysicalDevice& physicalDevice, vk::Device& device);
}  // namespace trg::vkwrappers