#pragma once

#include <memory>

namespace trg
{
	class VulkanContext;

	class VulkanContextFactory
	{
	public:
		VulkanContextFactory();

		std::unique_ptr<VulkanContext> makeContext() const;

	private:	
	};
}  // namespace trg