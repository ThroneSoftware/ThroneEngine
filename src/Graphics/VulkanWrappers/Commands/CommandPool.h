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
					std::size_t numberOfCommandBuffers,
					vk::CommandBufferLevel commandBufferLevel);

		CommandPool(const CommandPool& other) = delete;
		CommandPool& operator=(const CommandPool& other) = delete;

		// Moves are deleted because the auto generated version
		// destroys the command pool before the command buffers.
		CommandPool(CommandPool&& other) = delete;
		CommandPool& operator=(CommandPool&& other) = delete;

		~CommandPool() = default;

		VkHandleType& getVkHandle();
		const VkHandleType& getVkHandle() const;

		VkHandleType& operator*();
		const VkHandleType& operator*() const;

		VkHandleType* operator->();
		const VkHandleType* operator->() const;

		void addCommandBuffers(std::size_t numberOfCommandBuffers, vk::CommandBufferLevel commandBufferLevel);

		std::span<CommandBuffer> getAll();
		std::span<const CommandBuffer> getAll() const;

	private:
		std::reference_wrapper<vk::Device> m_device;

		vk::UniqueCommandPool m_commandPool;

		std::vector<CommandBuffer> m_commandBuffers;
	};
}  // namespace trg