#include "FencePool.h"

namespace trg
{
	FencePool::FencePool(vk::Device& device)
	  : m_device(device)
	{
	}

	FencePool::FencePool(vk::Device& device, std::size_t numberOfFences)
	  : m_device(device)
	{
		addFences(numberOfFences);
	}

	void FencePool::addFences(std::size_t numberOfFences)
	{
		assert(numberOfFences != 0);

		m_fences.reserve(m_fences.size() + numberOfFences);

		for(size_t i = 0; i < numberOfFences; ++i)
		{
			m_fences.emplace_back(m_device);
		}
	}

	void FencePool::wait(uint64_t timeout)
	{
		Fence::waitForFences(m_device, m_fences, true, timeout);
	}

	void FencePool::reset()
	{
		Fence::resetFences(m_device, m_fences);
	}

	std::span<Fence> FencePool::getAll()
	{
		return m_fences;
	}

	std::span<const Fence> FencePool::getAll() const
	{
		return m_fences;
	}
}  // namespace trg