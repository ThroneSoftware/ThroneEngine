#include "FencePool.h"

namespace trg
{
	FencePool::FencePool(vk::Device& device)
	  : m_device(device)
	{
	}

	FencePool::FencePool(vk::Device& device, int numberOfFences)
	  : m_device(device)
	{
		addFences(numberOfFences);
	}

	void FencePool::addFences(int numberOfFences)
	{
		assert(numberOfFences != 0);

		m_fences.reserve(m_fences.size() + numberOfFences);

		m_fences.emplace_back(m_device);
	}

	void FencePool::wait(uint64_t timeout)
	{
		Fence::waitForFences(m_device, m_fences, true, timeout);
	}

	void FencePool::reset()
	{
		Fence::resetFences(m_device, m_fences);
	}
}  // namespace trg