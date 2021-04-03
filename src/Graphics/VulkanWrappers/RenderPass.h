#pragma once

#include <Vulkan/vulkan.hpp>

namespace trg
{
	class RenderPass
	{
	public:
		RenderPass(vk::Device& device, vk::Format swapchainFormat);

		RenderPass(const RenderPass& other) = delete;
		RenderPass& operator=(const RenderPass& other) = delete;

		RenderPass(RenderPass&& other) = delete;
		RenderPass& operator=(RenderPass&& other) = delete;

		~RenderPass();

	private:
		vk::Device& m_device;

		vk::RenderPass m_renderPass;
	};
}  // namespace trg