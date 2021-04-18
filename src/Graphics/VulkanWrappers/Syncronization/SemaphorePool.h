#pragma once

#include "Semaphore.h"

#include <Vulkan/vulkan.hpp>

namespace trg
{
	class SemaphorePool
	{
	public:
		explicit SemaphorePool(vk::Device& device);
		SemaphorePool(vk::Device& device, int numberOfSemaphores);

		void addSemaphore(int numberOfSemaphores);

	private:
		vk::Device& m_device;

		std::vector<Semaphore> m_semaphores;
	};
}  // namespace trg