#pragma once

#include <Vulkan/vulkan.hpp>

#include <span>

namespace trg::vkwrappers
{
	class CommandPool;

	class CommandBuffer
	{
	public:
		using VkHandleType = vk::CommandBuffer;

	public:
		CommandBuffer() = default;
		CommandBuffer(vk::UniqueCommandBuffer commandBuffer);

		static void createCommandBuffers(vk::Device& device,
										 CommandPool& commandPool,
										 vk::CommandBufferLevel commandBufferLevel,
										 std::span<CommandBuffer> commandBuffersToAllocate);

		VkHandleType& getVkHandle();
		const VkHandleType& getVkHandle() const;

		VkHandleType& operator*();
		const VkHandleType& operator*() const;

		VkHandleType* operator->();
		const VkHandleType* operator->() const;

	private:
		vk::UniqueCommandBuffer m_commandBuffer;
	};
}  // namespace trg::vkwrappers