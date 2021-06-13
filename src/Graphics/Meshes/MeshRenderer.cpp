#include "MeshRenderer.h"

#include <gsl/gsl>

namespace trg
{
	namespace MeshRendererPrivate
	{
		vkwrappers::VertexBuffer makeVertexBuffer(const MeshFilter& meshFilter)
		{
			auto memory = tru::MemoryRegion(meshFilter.m_mesh.getAttributeData());

			auto vertexBuffer = vkwrappers::VertexBuffer(memory.byteSize,
														 vk::BufferUsageFlagBits::eVertexBuffer,
														 vma::MemoryUsage::eCpuToGpu,
														 0 /*bindingIndex*/);

			if(!meshFilter.m_mesh.getAttributeData().empty())
			{
				vertexBuffer.updateWithHostMemory(memory);
			}

			return vertexBuffer;
		}

		vkwrappers::IndexBuffer makeIndexBuffer(const MeshFilter& meshFilter)
		{
			auto memory = tru::MemoryRegion(meshFilter.m_mesh.getIndexData());

			auto indexBuffer = vkwrappers::IndexBuffer(memory.byteSize, vk::BufferUsageFlagBits::eIndexBuffer, vma::MemoryUsage::eCpuToGpu);

			if(!meshFilter.m_mesh.getIndexData().empty())
			{
				indexBuffer.updateWithHostMemory(memory);
			}

			return indexBuffer;
		}
	}  // namespace MeshRendererPrivate

	MeshRenderer::MeshRenderer(MeshFilter meshFilter)
	  : m_meshFilter(meshFilter)
	  , m_vertexBuffer(MeshRendererPrivate::makeVertexBuffer(m_meshFilter))
	  , m_indexBuffer(MeshRendererPrivate::makeIndexBuffer(m_meshFilter))
	{
	}

	void MeshRenderer::render(vkwrappers::BindableBindInfo& bindInfo)
	{
		m_vertexBuffer.bind(bindInfo);
		m_indexBuffer.bind(bindInfo);

		bindInfo.m_commandBuffer->drawIndexed(gsl::narrow<uint32_t>(m_meshFilter.m_mesh.getIndexData().size()),
											  1 /*instanceCount*/,
											  0 /*firstIndex*/,
											  0 /*vertexOffset*/,
											  0 /*firstInstance*/);
	}
}  // namespace trg