#pragma once

#include <Standard/Math/Transform.h>

namespace trc
{
	class FPCamera
	{
	public:
		FPCamera(trs::Transform& transform);

		void rotate(glm::vec2 rotation);
		void lookAt(const glm::vec3& target);

		glm::mat4 getViewMatrix() const;

	private:
		trs::Transform& m_transform;

		trs::Degree m_pitch;
	};
}  // namespace trc