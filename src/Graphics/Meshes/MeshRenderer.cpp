#include "MeshRenderer.h"
namespace trg
{
	namespace MeshRendererPrivate
	{
		vkwrappers::VertexBuffer makeVertexBuffer(const MeshFilter& meshFilter)
		{
			auto dataSize = meshFilter.m_mesh.getAttributeData().size() * sizeof(float);

			auto vertexBuffer =
				vkwrappers::VertexBuffer(dataSize, vk::BufferUsageFlagBits::eVertexBuffer, vma::MemoryUsage::eCpuToGpu, 0 /*bindingIndex*/);

			if(!meshFilter.m_mesh.getAttributeData().empty())
			{
				vertexBuffer.updateWithHostMemory(dataSize, &meshFilter.m_mesh.getAttributeData().front());
			}

			return vertexBuffer;
		}

		vkwrappers::IndexBuffer makeIndexBuffer(const MeshFilter& meshFilter)
		{
			auto dataSize = meshFilter.m_mesh.getIndexData().size();

			auto indexBuffer = vkwrappers::IndexBuffer(dataSize, vk::BufferUsageFlagBits::eVertexBuffer, vma::MemoryUsage::eCpuToGpu);

			if(!meshFilter.m_mesh.getIndexData().empty())
			{
				indexBuffer.updateWithHostMemory(dataSize, &meshFilter.m_mesh.getIndexData().front());
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

		bindInfo.commandBuffer->drawIndexed(m_meshFilter.m_mesh.getIndexData().size(),
											1 /*instanceCount*/,
											0 /*firstIndex*/,
											0 /*vertexOffset*/,
											0 /*firstInstance*/);
	}
}  // namespace trg