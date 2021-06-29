#include "PipelineDynamicState.h"

#include <Utilities/Overload.h>

namespace trg::vkwrappers
{
	PipelineDynamicState::PipelineDynamicState(State state)
	  : m_state(state)
	{
	}

	void PipelineDynamicState::bind(BindableBindInfo& bindInfo) const
	{
		std::visit(tru::Overload(
					   [&bindInfo](const ViewportState& state) {
						   bindInfo.m_commandBuffer->setViewport(0, state.m_viewport);
					   },
					   [&bindInfo](const ScissorState& state) {
						   bindInfo.m_commandBuffer->setScissor(0, state.m_scissor);
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

	void PipelineDynamicStates::bind(BindableBindInfo& bindInfo) const
	{
		for(const auto& state: m_states)
		{
			state.bind(bindInfo);
		}
	}

	void PipelineDynamicStates::insertOrReplace(const PipelineDynamicState& state)
	{
		auto found = std::find_if(m_states.begin(), m_states.end(), [&state](const PipelineDynamicState& value) {
			return value.getDynamicState() == state.getDynamicState();
		});

		if(found != m_states.end())
		{
			*found = state;
		}
		else
		{
			m_states.emplace_back(state);
		}
	}

	void PipelineDynamicStates::insertOrReplace(const PipelineDynamicStates& states)
	{
		for(const auto& state: states.m_states)
		{
			insertOrReplace(state);
		}
	}

	void PipelineDynamicStates::remove(const PipelineDynamicState& state)
	{
		auto found = std::find_if(m_states.begin(), m_states.end(), [&state](const PipelineDynamicState& value) {
			return value.getDynamicState() == state.getDynamicState();
		});

		if(found != m_states.end())
		{
			m_states.erase(found);
		}
	}

	void PipelineDynamicStates::remove(const PipelineDynamicStates& states)
	{
		for (const auto& state : states.m_states)
		{
			remove(state);
		}
	}

	void PipelineDynamicStates::clear()
	{
		m_states.clear();
	}
}  // namespace trg::vkwrappers