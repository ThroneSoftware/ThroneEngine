target_sources(Graphics PRIVATE 
	GraphicsContext.cpp
	GraphicsContext.h
	GraphicsInstance.cpp
	GraphicsInstance.h
	Utility.h
	VulkanContext.cpp
	VulkanContext.h
	VulkanContextFactory.cpp
	VulkanContextFactory.h
	VulkanWrappers/FrameBuffer.cpp
	VulkanWrappers/FrameBuffer.h
	VulkanWrappers/Memory/Buffer.cpp
	VulkanWrappers/Memory/Buffer.h
	VulkanWrappers/Memory/Image.cpp
	VulkanWrappers/Memory/Image.h
	VulkanWrappers/Memory/ImageView.cpp
	VulkanWrappers/Memory/ImageView.h
	VulkanWrappers/Memory/VmaAllocator.cpp
	VulkanWrappers/Memory/VmaAllocator.h
	VulkanWrappers/Memory/VmaUnique.h
	VulkanWrappers/RenderPass.cpp
	VulkanWrappers/RenderPass.h
	VulkanWrappers/Swapchain.cpp
	VulkanWrappers/Swapchain.h
)