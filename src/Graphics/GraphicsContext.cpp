#include "GraphicsContext.h"

#include "VulkanContext.h"

namespace trg
{
	GraphicsContext::GraphicsContext(const VulkanContextFactory& initializer)
	  : m_vulkanContext(initializer.makeContext())
	{
	}

	GraphicsContext::~GraphicsContext()
	{
	}
}  // namespace trg
