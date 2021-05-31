#include "Semaphore.h"

namespace trg::vkwrappers
{
	namespace SemaphorePrivate
	{
		auto makeSemaphore(vk::Device& device)
		{
			auto createInfo = vk::SemaphoreCreateInfo();
			return device.createSemaphoreUnique(createInfo);
		}
	}  // namespace SemaphorePrivate

	Semaphore::Semaphore(vk::Device& device)
	  : m_semaphore(SemaphorePrivate::makeSemaphore(device))
	{
	}

	Semaphore::VkHandleType& Semaphore::getVkHandle()
	{
		return m_semaphore.get();
	}

	const Semaphore::VkHandleType& Semaphore::getVkHandle() const
	{
		return m_semaphore.get();
	}

	Semaphore::VkHandleType& Semaphore::operator*()
	{
		return getVkHandle();
	}

	const Semaphore::VkHandleType& Semaphore::operator*() const
	{
		return getVkHandle();
	}

	Semaphore::VkHandleType* Semaphore::operator->()
	{
		return &getVkHandle();
	}

	const Semaphore::VkHandleType* Semaphore::operator->() const
	{
		return &getVkHandle();
	}
}  // namespace trg::vkwrappers