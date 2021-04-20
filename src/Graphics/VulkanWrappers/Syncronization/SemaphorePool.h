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

		void addSemaphores(int numberOfSemaphores);

		std::span<Semaphore> getAll();
		std::span<const Semaphore> getAll() const;

	private:
		vk::Device& m_device;

		std::vector<Semaphore> m_semaphores;
	};
}  // namespace trg