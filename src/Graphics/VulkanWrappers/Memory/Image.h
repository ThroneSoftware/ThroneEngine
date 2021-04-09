#pragma once

#include <Vendors/VulkanMemoryAllocator/vk_mem_alloc.hpp>
#include <Vulkan/vulkan.hpp>

namespace trg
{
	class Image
	{
	public:
		Image();

		Image(const Image& other) = delete;
		Image& operator=(const Image& other) = delete;

		Image(Image&& other) = delete;
		Image& operator=(Image&& other) = delete;

		~Image();

	private:
		Image(std::pair<vk::Image, vma::Allocation>&& image);

		vk::Image m_image;
		vma::Allocation m_allocation;
	};
}  // namespace trg