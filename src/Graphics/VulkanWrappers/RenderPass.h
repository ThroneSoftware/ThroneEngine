#pragma once

#include <Vulkan/vulkan.hpp>

namespace trg
{
	class RenderPass
	{
	public:
		using VkHandleType = vk::RenderPass;

	public:
		RenderPass(vk::Device& device, vk::Format swapchainFormat);

		VkHandleType& getVkHandle();
		const VkHandleType& getVkHandle() const;

		VkHandleType& operator*();
		const VkHandleType& operator*() const;

		VkHandleType* operator->();
		const VkHandleType* operator->() const;

	private:
		vk::UniqueRenderPass m_renderPass;
	};
}  // namespace trg