#pragma once

#include "GraphicsContext.h"

namespace trg
{
	class VulkanContext;

	class GraphicsInstance
	{
	public:
		GraphicsInstance(std::unique_ptr<GraphicsContext> context);

		void present();

	private:
		VulkanContext& vulkanContext();

		std::unique_ptr<GraphicsContext> m_context;
	};
}  // namespace trg