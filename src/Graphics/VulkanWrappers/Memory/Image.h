#pragma once

#include "VmaUnique.h"

#include <Vendors/VulkanMemoryAllocator/vk_mem_alloc.hpp>
#include <Vulkan/vulkan.hpp>

namespace trg
{
	class Image
	{
	public:
		Image();
	private:
		VmaUnique<vk::Image> m_image;
	};
}  // namespace trg