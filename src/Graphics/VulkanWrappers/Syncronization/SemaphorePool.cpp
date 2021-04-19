#include "SemaphorePool.h"

namespace trg
{
	SemaphorePool::SemaphorePool(vk::Device& device)
	  : m_device(device)
	{
	}

	SemaphorePool::SemaphorePool(vk::Device& device, int numberOfSemaphores)
	  : m_device(device)
	{
		addSemaphores(numberOfSemaphores);
	}

	void SemaphorePool::addSemaphores(int numberOfSemaphores)
	{
		assert(numberOfSemaphores != 0);

		m_semaphores.reserve(m_semaphores.size() + numberOfSemaphores);

		for(size_t i = 0; i < numberOfSemaphores; ++i)
		{
			m_semaphores.emplace_back(m_device);
		}
	}
}  // namespace trg