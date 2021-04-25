#include "Fence.h"

#include "../../Utility.h"

namespace trg
{
	namespace FencePrivate
	{
		auto makeFence(vk::Device& device)
		{
			auto createInfo = vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled);
			return device.createFenceUnique(createInfo);
		}
	}  // namespace FencePrivate

	Fence::Fence(vk::Device& device)
	  : m_fence(FencePrivate::makeFence(device))
	  , m_device(device)
	{
	}

	Fence::VkHandleType& Fence::getVkHandle()
	{
		return m_fence.get();
	}

	const Fence::VkHandleType& Fence::getVkHandle() const
	{
		return m_fence.get();
	}

	Fence::VkHandleType& Fence::operator*()
	{
		return getVkHandle();
	}

	const Fence::VkHandleType& Fence::operator*() const
	{
		return getVkHandle();
	}

	Fence::VkHandleType* Fence::operator->()
	{
		return &getVkHandle();
	}

	const Fence::VkHandleType* Fence::operator->() const
	{
		return &getVkHandle();
	}

	void Fence::wait(uint64_t timeout)
	{
		auto result = m_device.get().waitForFences(getVkHandle(), true, timeout);
	}

	void Fence::reset()
	{
		m_device.get().resetFences(getVkHandle());
	}

	void Fence::waitForFences(vk::Device& device, std::span<Fence> fences, bool waitForAll, uint64_t timeout)
	{
		auto vkHandles = toVkHandle(fences);
		auto result = device.waitForFences(vkHandles, waitForAll, timeout);
	}

	void Fence::resetFences(vk::Device& device, std::span<Fence> fences)
	{
		auto vkHandles = toVkHandle(fences);
		device.resetFences(vkHandles);
	}
}  // namespace trg