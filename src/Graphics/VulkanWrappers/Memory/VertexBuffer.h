#include "../Bindable/Bindable.h"
#include "Buffer.h"

namespace trg::vkwrappers
{
	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(vk::DeviceSize bufferSize, vk::BufferUsageFlagBits bufferUsage, vma::MemoryUsage memoryUsage, uint32_t bindingIndex);

		void bind(BindableBindInfo& bindInfo);

	private:
		uint32_t m_bindingIndex;
	};
}  // namespace trg::vkwrappers