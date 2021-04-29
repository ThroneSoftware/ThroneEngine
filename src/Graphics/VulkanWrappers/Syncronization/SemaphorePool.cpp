#include "SemaphorePool.h"

namespace trg
{
	SemaphorePool::SemaphorePool(vk::Device& device)
	  : m_device(device)
	{
	}

	SemaphorePool::SemaphorePool(vk::Device& device, std::size_t numberOfSemaphores)
	  : m_device(device)
	{
		addSemaphores(numberOfSemaphores);
	}

	void SemaphorePool::addSemaphores(std::size_t numberOfSemaphores)
	{
		assert(numberOfSemaphores != 0);

		m_semaphores.reserve(m_semaphores.size() + numberOfSemaphores);

		for(size_t i = 0; i < numberOfSemaphores; ++i)
		{
			m_semaphores.emplace_back(m_device);
		}
	}

	std::span<Semaphore> SemaphorePool::getAll()
	{
		return m_semaphores;
	}

	std::span<const Semaphore> SemaphorePool::getAll() const
	{
		return m_semaphores;
	}
}  // namespace trg