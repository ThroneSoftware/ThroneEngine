#pragma once

#include "../Bindable/Bindable.h"

#include <Vulkan/vulkan.hpp>

#include <variant>

namespace trg::vkwrappers
{
	class PipelineDynamicState
	{
		struct ViewportState
		{
			ViewportState() = default;
			ViewportState(vk::Viewport viewport)
			  : m_viewport(viewport)
			{
			}

			vk::Viewport m_viewport;
		};

		struct ScissorState
		{
			ScissorState() = default;
			ScissorState(vk::Rect2D scissor)
			  : m_scissor(scissor)
			{
			}

			vk::Rect2D m_scissor;
		};

		using State = std::variant<ViewportState, ScissorState>;

	public:
		PipelineDynamicState(State state);

		void bind(BindableBindInfo& bindInfo) const;

		vk::DynamicState getDynamicState() const;

	private:
		State m_state;
	};

	class PipelineDynamicStates
	{
	public:
		void bind(BindableBindInfo& bindInfo) const;

		void insertOrReplace(const PipelineDynamicState& state);
		void insertOrReplace(const PipelineDynamicStates& states);

		void remove(const PipelineDynamicState& state);
		void remove(const PipelineDynamicStates& states);

		void clear();

	private:
		std::vector<PipelineDynamicState> m_states;
	};
}  // namespace trg::vkwrappers