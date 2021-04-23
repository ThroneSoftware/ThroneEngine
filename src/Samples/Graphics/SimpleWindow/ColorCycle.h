#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>

#include <array>

namespace trg
{
	class ColorCycle
	{
	public:
		ColorCycle()
		{
			auto& clearColors = getClearColors();

			m_currentColor = clearColors[m_currentColorId];
			m_nextColor = clearColors[m_nextColorId];
		}

		glm::vec3 getClearColor(float deltaTime)
		{
			auto& clearColors = getClearColors();

			m_cosValue += m_colorSpeed * deltaTime;

			// range: [0, 1]
			auto lerpValue = (std::cos(m_cosValue) + 1) / 2.0f;

			if(m_cosValue > 2 * glm::pi<float>())
			{
				m_cosValue = glm::pi<float>();
				lerpValue = (std::sin(m_cosValue) + 1) / 2.0f;

				m_currentColorId = m_nextColorId;
				m_nextColorId = (m_currentColorId + 1) % (clearColors.size() - 1);

				m_currentColor = m_nextColor;
				m_nextColor = clearColors[m_nextColorId];
			}

			return glm::lerp(m_currentColor, m_nextColor, lerpValue);
		}

	private:
		const std::array<glm::vec3, 6>& getClearColors()
		{
			static constexpr auto clearColors = std::array{glm::vec3{1.0f, 0.0f, 0.0f},
														   glm::vec3{1.0f, 1.0f, 0.0f},
														   glm::vec3{0.0f, 1.0f, 0.0f},
														   glm::vec3{0.0f, 1.0f, 1.0f},
														   glm::vec3{0.0f, 0.0f, 1.0f},
														   glm::vec3{1.0f, 0.0f, 1.0f}};

			return clearColors;
		}

		int m_currentColorId = 0;
		int m_nextColorId = 1;

		glm::vec3 m_currentColor;
		glm::vec3 m_nextColor;

		float m_cosValue = glm::pi<float>();
		float m_colorSpeed = 0.001f;
	};
}  // namespace trg