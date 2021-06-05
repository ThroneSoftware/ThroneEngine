#pragma once

#include "../Bindable/Bindable.h"
#include "Buffer.h"

namespace trg::vkwrappers
{
	class IndexBuffer : public Buffer
	{
	public:
		IndexBuffer(vk::DeviceSize bufferSize, vk::BufferUsageFlagBits bufferUsage, vma::MemoryUsage memoryUsage);

		void bind(BindableBindInfo& bindInfo);

	private:
	};
}  // namespace trg::vkwrappers