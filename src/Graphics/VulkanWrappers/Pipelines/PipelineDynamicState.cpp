#include "PipelineDynamicState.h"

#include <Utilities/Overload.h>

namespace trg::vkwrappers
{
	PipelineDynamicState::PipelineDynamicState(State state)
	  : m_state(state)
	{
	}

	void PipelineDynamicState::bind(BindableBindInfo& bindInfo)
	{
		std::visit(tru::Overload(
					   [&bindInfo](const ViewportState& state) {
						   bindInfo.m_commandBuffer->setViewport(0, state.m_viewport);
					   },
					   [&bindInfo](const ScissorState& state) {
						   return vk::DynamicState::eScissor;
					   }),
				   m_state);
	}

	vk::DynamicState PipelineDynamicState::getDynamicState() const
	{
		return std::visit(tru::Overload(
							  [](const ViewportState&) {
								  return vk::DynamicState::eViewport;
							  },
							  [](const ScissorState&) {
								  return vk::DynamicState::eScissor;
							  }),
						  m_state);
	}
}  // namespace trg::vkwrappers