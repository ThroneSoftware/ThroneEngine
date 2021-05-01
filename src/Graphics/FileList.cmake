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
	VulkanContextFactoryFunctions.h
	VulkanWrappers/Commands/CommandBuffer.cpp
	VulkanWrappers/Commands/CommandBuffer.h
	VulkanWrappers/Commands/CommandBufferRecordScope.cpp
	VulkanWrappers/Commands/CommandBufferRecordScope.h
	VulkanWrappers/Commands/CommandPool.cpp
	VulkanWrappers/Commands/CommandPool.h
	VulkanWrappers/Commands/CommandQueue.cpp
	VulkanWrappers/Commands/CommandQueue.h
	VulkanWrappers/Commands/RenderPassRecordScope.cpp
	VulkanWrappers/Commands/RenderPassRecordScope.h
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
	VulkanWrappers/Syncronization/Fence.cpp
	VulkanWrappers/Syncronization/Fence.h
	VulkanWrappers/Syncronization/FencePool.cpp
	VulkanWrappers/Syncronization/FencePool.h
	VulkanWrappers/Syncronization/Semaphore.cpp
	VulkanWrappers/Syncronization/Semaphore.h
	VulkanWrappers/Syncronization/SemaphorePool.cpp
	VulkanWrappers/Syncronization/SemaphorePool.h
)