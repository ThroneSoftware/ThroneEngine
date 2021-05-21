#include <Graphics/GraphicsInstance.h>

#include <Graphics/VulkanContext.h>
#include <Graphics/VulkanContextFactory.h>
#include <Graphics/VulkanWrappers/Commands/CommandBufferRecordScope.h>
#include <Graphics/VulkanWrappers/Commands/CommandPool.h>
#include <Graphics/VulkanWrappers/Commands/RenderPassRecordScope.h>
#include <Graphics/VulkanWrappers/FrameBuffer.h>
#include <Graphics/VulkanWrappers/Memory/Image.h>
#include <Graphics/VulkanWrappers/Memory/VertexBuffer.h>
#include <Graphics/VulkanWrappers/Pipelines/GraphicsPipeline.h>
#include <Graphics/VulkanWrappers/RenderPass.h>
#include <Graphics/VulkanWrappers/Shaders/Shader.h>
#include <Graphics/VulkanWrappers/Syncronization/FencePool.h>
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <gsl/gsl>

#include <iostream>

trg::Image makeDepthImage(trg::VulkanContext& vkContext)
{
	auto depthImage = trg::Image(vkContext.m_device,
								 vk::ImageType::e2D,
								 vk::Format::eD32Sfloat,
								 vk::Extent3D(vkContext.m_swapchainExtent, 1),
								 1,
								 1,
								 vk::SampleCountFlagBits::e1,
								 vk::ImageUsageFlagBits::eDepthStencilAttachment,
								 vk::ImageLayout::eUndefined);

	depthImage.addImageView(vk::ImageAspectFlagBits::eDepth, vk::ImageViewType::e2D, vk::Format::eD32Sfloat, 0, 1);

	return depthImage;
}

trg::RenderPass makeRenderPass(trg::VulkanContext& vkContext)
{
	return trg::RenderPass(vkContext.m_device, vkContext.m_swapchain.getFormat());
}

trg::FrameBuffer
	makeFrameBuffer(trg::VulkanContext& vkContext, trg::ImageView& swapchainImageView, trg::Image& depthImage, trg::RenderPass& renderPass)
{
	std::vector<vk::ImageView> attachments = {*swapchainImageView, *depthImage.getImageView()};
	return trg::FrameBuffer(vkContext.m_device, renderPass, attachments, vkContext.m_swapchainExtent, 1);
}

trg::GraphicsPipeline makeGraphicsPipeline(trg::VulkanContext& vkContext, trg::RenderPass& renderPass, std::span<const trg::Shader> shaders)
{
	std::vector<trg::DescriptorSetLayout> descriptorSetLayouts;

	std::vector<trg::VertexBufferSignature> vertexBufferSignatures;
	vertexBufferSignatures.emplace_back(
		trg::VertexBufferSignature(0,
								   vk::VertexInputRate::eVertex,
								   {trg::VertexBufferBlock(sizeof(glm::vec3), 1 /*count*/, 0 /*location*/, vk::Format::eR32G32B32Sfloat),
									trg::VertexBufferBlock(sizeof(glm::vec3), 1 /*count*/, 1 /*location*/, vk::Format::eR32G32B32Sfloat)}));

	auto inputAssemblyState =
		vk::PipelineInputAssemblyStateCreateInfo({}, vk::PrimitiveTopology::eTriangleList, false /*primitiveRestartEnable*/);

	auto viewport = vk::Viewport(0.0f /*x*/,
								 static_cast<float>(vkContext.m_swapchainExtent.height),
								 static_cast<float>(vkContext.m_swapchainExtent.width),
								 -static_cast<float>(vkContext.m_swapchainExtent.height),
								 0.0f /*minDepth*/,
								 1.0f /*maxDepth*/);
	auto scissor = vk::Rect2D(vk::Offset2D(0 /*x*/, 0 /*y*/), vkContext.m_swapchainExtent);
	auto viewportState = vk::PipelineViewportStateCreateInfo({}, viewport, scissor);

	auto rasterizationState = vk::PipelineRasterizationStateCreateInfo({},
																	   false /*depthClampEnable*/,
																	   false /*rasterizerDiscardEnable*/,
																	   vk::PolygonMode::eFill,
																	   vk::CullModeFlagBits::eNone,
																	   vk::FrontFace::eCounterClockwise,
																	   false /*depthBiasEnable*/,
																	   0.0f /*depthBiasConstantFactor*/,
																	   0.0f /*depthBiasClamp*/,
																	   0.0f /*depthBiasSlopeFactor*/,
																	   1.0f /*lineWidth*/);

	auto multisampleState = vk::PipelineMultisampleStateCreateInfo({},
																   vk::SampleCountFlagBits::e1,
																   false /*sampleShadingEnable*/,
																   0.0f /*minSampleShading*/,
																   nullptr /*pSampleMask*/,
																   false /*alphaToCoverageEnable*/,
																   false /*alphaToOneEnable*/);

	auto depthStencilState = vk::PipelineDepthStencilStateCreateInfo({},
																	 true /*depthTestEnable*/,
																	 true /*depthWriteEnable*/,
																	 vk::CompareOp::eLess,
																	 false /*depthBoundsTestEnable*/,
																	 false /*stencilTestEnable*/,
																	 {} /*front*/,
																	 {} /*back*/,
																	 0.0f /*minDepthBounds*/,
																	 1.0f /*maxDepthBounds*/);


	// clang-format off
	std::vector<vk::PipelineColorBlendAttachmentState> attachmentBlendStates = {
		vk::PipelineColorBlendAttachmentState(
			false, /*blendEnable*/
			vk::BlendFactor::eOne, /*srcColorBlendFactor*/
			vk::BlendFactor::eOne, /*dstColorBlendFactor*/
			vk::BlendOp::eAdd, /*colorBlendOp*/
			vk::BlendFactor::eOne, /*srcAlphaBlendFactor*/
			vk::BlendFactor::eOne, /*dstAlphaBlendFactor*/
			vk::BlendOp::eAdd, /*alphaBlendOp*/
			vk::ColorComponentFlagBits::eR | /*colorWriteMask*/
			vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eB |
			vk::ColorComponentFlagBits::eA
		)
	};
	// clang-format on

	auto colorBlendState = vk::PipelineColorBlendStateCreateInfo({},
																 false /*blendLogicOpEnabled*/,
																 vk::LogicOp::eCopy,
																 attachmentBlendStates,
																 std::array{0.0f, 0.0f, 0.0f, 0.0f});

	return trg::GraphicsPipeline(vkContext.m_device,
								 descriptorSetLayouts,
								 {} /*pushConstants*/,
								 shaders,
								 vertexBufferSignatures,
								 &inputAssemblyState,
								 nullptr /*tesselationState*/,
								 &viewportState,
								 &rasterizationState,
								 &multisampleState,
								 &depthStencilState,
								 &colorBlendState,
								 nullptr /*dynamicState*/,
								 renderPass);
}

trg::Fence makeFence(trg::VulkanContext& vkContext)
{
	return trg::Fence(vkContext.m_device);
}

trg::Semaphore makeSemaphore(trg::VulkanContext& vkContext)
{
	return trg::Semaphore(vkContext.m_device);
}

class FrameContext
{
public:
	FrameContext(trg::GraphicsInstance& graphicsInstance,
				 trg::CommandBuffer& commandBuffer,
				 trg::ImageView& swapchainImageView,
				 trg::RenderPass& renderPass,
				 trg::GraphicsPipeline& graphicsPipeline,
				 trg::VertexBuffer& vertexBuffer)
	  : m_graphicsInstance(graphicsInstance)
	  , m_vkContext(m_graphicsInstance.vulkanContext())
	  , m_commandBuffer(commandBuffer)
	  , m_swapchainImageView(swapchainImageView)
	  , m_renderPass(renderPass)
	  , m_graphicsPipeline(graphicsPipeline)
	  , m_vertexBuffer(vertexBuffer)
	  , m_depthImage(makeDepthImage(m_vkContext))
	  , m_frameBuffer(makeFrameBuffer(m_vkContext, m_swapchainImageView, m_depthImage, m_renderPass))
	  , m_submitCommandBufferFinishedFence(makeFence(m_vkContext))
	  , m_submitCommandBufferFinishedSemaphore(makeSemaphore(m_vkContext))
	{
	}

	void renderFrame(glm::vec3 clearColor, std::uint32_t imageIndex, trg::Semaphore& acquireNextImageSemaphore)
	{
		m_submitCommandBufferFinishedFence.wait();
		m_submitCommandBufferFinishedFence.reset();

		{
			auto cmdScope = trg::CommandBufferRecordScope(m_commandBuffer, vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

			auto renderPassScope = trg::RenderPassRecordScope(m_commandBuffer, m_vkContext, m_renderPass, m_frameBuffer, clearColor);

			trg::BindableBindInfo bindableBindInfo = {.commandBuffer = m_commandBuffer};

			m_graphicsPipeline.bind(bindableBindInfo);

			m_vertexBuffer.bind(bindableBindInfo);

			m_commandBuffer->draw(3 /*vertexCount*/, 1 /*instanceCount*/, 0 /*firstVertex*/, 0 /*firstInstance*/);
		}

		std::vector submitWaitSemaphores = {*acquireNextImageSemaphore};
		m_vkContext.m_graphicsQueue.submitCommandBuffer(submitWaitSemaphores,
														{vk::PipelineStageFlagBits::eAllGraphics},
														m_commandBuffer,
														std::span(&m_submitCommandBufferFinishedSemaphore, 1),
														m_submitCommandBufferFinishedFence);

		std::vector presentWaitSemaphores = {*m_submitCommandBufferFinishedSemaphore};
		m_graphicsInstance.present(imageIndex, presentWaitSemaphores);
	}

private:
	// Order of variables is important for member initializer list.

	trg::GraphicsInstance& m_graphicsInstance;
	trg::VulkanContext& m_vkContext;

	trg::CommandBuffer& m_commandBuffer;

	trg::ImageView& m_swapchainImageView;

	trg::RenderPass& m_renderPass;

	trg::GraphicsPipeline& m_graphicsPipeline;

	trg::VertexBuffer& m_vertexBuffer;

	trg::Image m_depthImage;

	trg::FrameBuffer m_frameBuffer;

	trg::Fence m_submitCommandBufferFinishedFence;
	trg::Semaphore m_submitCommandBufferFinishedSemaphore;
};

auto makeFrameContexts(std::size_t frameContextCount,
					   trg::GraphicsInstance& instance,
					   trg::CommandPool& commandBuffers,
					   trg::RenderPass& renderPass,
					   trg::GraphicsPipeline& graphicsPipeline,
					   trg::VertexBuffer& vertexBuffer)
{
	std::vector<FrameContext> frameContexts;
	frameContexts.reserve(frameContextCount);

	for(size_t i = 0; i < frameContextCount; ++i)
	{
		frameContexts.emplace_back(instance,
								   commandBuffers.getAll()[i],
								   instance.vulkanContext().m_swapchain.getImageViews()[i],
								   renderPass,
								   graphicsPipeline,
								   vertexBuffer);
	}

	return frameContexts;
}

int main()
{
	std::filesystem::current_path(std::filesystem::absolute(__FILE__).parent_path());

	auto instance = trg::GraphicsInstance(std::make_unique<trg::GraphicsContext>(trg::VulkanContextFactory()));
	auto& vkContext = instance.vulkanContext();

	auto frameContextCount = vkContext.m_swapchain.getImageViews().size();

	auto commandBuffers = trg::CommandPool(vkContext.m_device,
										   vkContext.m_graphicsQueue,
										   vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
										   frameContextCount,
										   vk::CommandBufferLevel::ePrimary);

	auto renderPass = makeRenderPass(vkContext);

	auto acquireNextImageSemaphores = trg::SemaphorePool(vkContext.m_device, frameContextCount);

	std::vector<trg::Shader> shaders;
	shaders.emplace_back(vkContext.m_device, "TriangleVert.spv", vk::ShaderStageFlagBits::eVertex);
	shaders.emplace_back(vkContext.m_device, "TriangleFrag.spv", vk::ShaderStageFlagBits::eFragment);

	auto graphicsPipeline = makeGraphicsPipeline(vkContext, renderPass, shaders);

	// layout : vertex, color
	std::vector<glm::vec3> triangleVertices =
		{{0.0f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}};

	auto dataSize = triangleVertices.size() * sizeof(glm::vec3);
	auto vertexBuffer =
		trg::VertexBuffer(dataSize, vk::BufferUsageFlagBits::eVertexBuffer, vma::MemoryUsage::eCpuToGpu, 0 /*bindingIndex*/);
	vertexBuffer.updateWithHostMemory(dataSize, triangleVertices.data());

	std::vector<FrameContext> frameContexts =
		makeFrameContexts(frameContextCount, instance, commandBuffers, renderPass, graphicsPipeline, vertexBuffer);

	float deltaTime = 0;

	uint64_t frameId = 0;
	while(!instance.windowShouldClose())
	{
		auto currentResourceIndex = frameId % frameContextCount;

		auto begin = std::chrono::steady_clock::now();

		instance.processWindowEvents();

		if(!vkContext.windowMinimized)
		{
			if(bool expected = true; vkContext.hasWindowResizeEvent.compare_exchange_strong(expected, false))
			{
				vkContext.m_device.waitIdle();

				graphicsPipeline = makeGraphicsPipeline(vkContext, renderPass, shaders);

				frameContexts = makeFrameContexts(frameContextCount, instance, commandBuffers, renderPass, graphicsPipeline, vertexBuffer);
			}

			auto clearColor = glm::vec3(0.0f);

			auto& acquireNextImageSemaphore = acquireNextImageSemaphores.getAll()[currentResourceIndex];
			auto imageIndex = vkContext.m_swapchain.acquireImage(acquireNextImageSemaphore);

			FrameContext& frameContext = frameContexts[imageIndex];
			frameContext.renderFrame(clearColor, imageIndex, acquireNextImageSemaphore);
		}

		frameId++;

		auto end = std::chrono::steady_clock::now();

		deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - begin).count();
	}

	vkContext.m_device.waitIdle();
}