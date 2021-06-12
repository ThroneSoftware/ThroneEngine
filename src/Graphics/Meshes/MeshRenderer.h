#pragma once

#include "../VulkanWrappers/Bindable/Bindable.h"
#include "../VulkanWrappers/Memory/IndexBuffer.h"
#include "../VulkanWrappers/Memory/VertexBuffer.h"
#include "MeshFilter.h"

namespace trg
{
	class MeshRenderer
	{
	public:
		MeshRenderer(MeshFilter meshFilter);

		void render(vkwrappers::BindableBindInfo& bindInfo);

	private:
		MeshFilter m_meshFilter;

		vkwrappers::VertexBuffer m_vertexBuffer;
		vkwrappers::IndexBuffer m_indexBuffer;
	};
}  // namespace trg