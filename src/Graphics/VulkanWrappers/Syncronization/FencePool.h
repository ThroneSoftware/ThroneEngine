#pragma once

#include "Fence.h"

#include <Vulkan/vulkan.hpp>

namespace trg::vkwrappers
{
	class FencePool
	{
	public:
		explicit FencePool(vk::Device& device);
		FencePool(vk::Device& device, std::size_t numberOfFences);

		void addFences(std::size_t numberOfFences);
		void wait(uint64_t timeout = std::numeric_limits<uint64_t>::max());
		void reset();

		std::span<Fence> getAll();
		std::span<const Fence> getAll() const;

	private:
		vk::Device& m_device;

		std::vector<Fence> m_fences;
	};
}  // namespace trg::vkwrappers