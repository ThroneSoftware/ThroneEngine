target_sources(Graphics PRIVATE 
	GraphicsContext.cpp
	GraphicsContext.h
	GraphicsInstance.cpp
	GraphicsInstance.h
	VulkanContext.cpp
	VulkanContext.h
	VulkanContextFactory.cpp
	VulkanContextFactory.h
	VulkanWrappers/Memory/Buffer.cpp
	VulkanWrappers/Memory/Buffer.h
	VulkanWrappers/Memory/Image.cpp
	VulkanWrappers/Memory/Image.h
	VulkanWrappers/Memory/VmaAllocator.h
	VulkanWrappers/RenderPass.cpp
	VulkanWrappers/RenderPass.h
	VulkanWrappers/Swapchain.cpp
	VulkanWrappers/Swapchain.h
)