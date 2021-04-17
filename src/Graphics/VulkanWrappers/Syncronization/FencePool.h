#pragma once

#include "Fence.h"

#include <Vulkan/vulkan.hpp>

namespace trg
{
	class FencePool
	{
	public:
		FencePool(vk::Device& device, int numberOfFences);

		void addFences(int numberOfFences);
		void wait(uint64_t timeout = std::numeric_limits<uint64_t>::max());
		void reset();

	private:
		vk::Device& m_device;
		std::vector<Fence> m_fences;
	};
}  // namespace trg