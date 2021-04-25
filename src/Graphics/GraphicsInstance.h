#pragma once

#include "GraphicsContext.h"
#include "VulkanWrappers/Syncronization/Semaphore.h"

namespace trg
{
	class VulkanContext;

	class GraphicsInstance
	{
	public:
		GraphicsInstance(std::unique_ptr<GraphicsContext> context);

		void processWindowEvents();

		void present(uint32_t imageIndex, std::vector<vk::Semaphore>& waitSemaphores);

		VulkanContext& vulkanContext();

		bool windowShouldClose();

	private:
		std::unique_ptr<GraphicsContext> m_context;
	};
}  // namespace trg