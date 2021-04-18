#pragma once

#include <Vulkan/vulkan.hpp>

namespace trg
{
	class Semaphore
	{
	public:
		using VkHandleType = vk::Semaphore;

	public:
		explicit Semaphore(vk::Device& device);

		VkHandleType& getVkHandle();
		const VkHandleType& getVkHandle() const;

		VkHandleType& operator*();
		const VkHandleType& operator*() const;

		VkHandleType* operator->();
		const VkHandleType* operator->() const;

	private:
		vk::UniqueSemaphore m_semaphore;
	};
}  // namespace trg