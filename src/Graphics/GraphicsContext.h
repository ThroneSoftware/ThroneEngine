#pragma once

#include "VulkanContextFactory.h"

#include <memory>

namespace trg
{
	class VulkanContext;

	class GraphicsContext
	{
	public:
		GraphicsContext(const VulkanContextFactory& initializer);

		~GraphicsContext();

	private:
		std::unique_ptr<VulkanContext> m_vulkanContext;
	};
}  // namespace trg