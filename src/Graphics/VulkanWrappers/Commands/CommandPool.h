#pragma once

#include "CommandBuffer.h"

#include <Vulkan/vulkan.hpp>

namespace trg
{
	class CommandQueue;

	class CommandPool
	{
	public:
		using VkHandleType = vk::CommandPool;

	public:
		CommandPool(vk::Device& device,
					const CommandQueue& commandQueue,
					vk::CommandPoolCreateFlagBits flags,
					int numberOfCommandBuffers,
					vk::CommandBufferLevel commandBufferLevel);

		VkHandleType& getVkHandle();
		const VkHandleType& getVkHandle() const;

		VkHandleType& operator*();
		const VkHandleType& operator*() const;

		VkHandleType* operator->();
		const VkHandleType* operator->() const;

		void addCommandBuffers(int numberOfCommandBuffers, vk::CommandBufferLevel commandBufferLevel);

		std::span<CommandBuffer> getAll();
		std::span<const CommandBuffer> getAll() const;

	private:
		vk::Device& m_device;

		vk::UniqueCommandPool m_commandPool;

		std::vector<CommandBuffer> m_commandBuffers;
	};
}  // namespace trg