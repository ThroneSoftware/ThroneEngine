#include "FPCamera.h"

#include <iostream>

namespace trc
{
	FPCamera::FPCamera(trs::Transform& transform)
	  : m_transform(transform)
	{
	}

	void FPCamera::rotate(glm::vec2 rotation)
	{
		constexpr auto maxPitch = trs::Degree(89.5f);
		constexpr auto minPitch = trs::Degree(-89.5f);

		auto pitchRotation = trs::Degree(rotation.y);
		auto yawRotation = trs::Degree(-rotation.x);

		m_pitch += pitchRotation;
		m_yaw += yawRotation;

		if(m_pitch >= maxPitch)
		{
			pitchRotation -= m_pitch - maxPitch;
		}
		else if(m_pitch <= minPitch)
		{
			pitchRotation -= m_pitch - minPitch;
		}
		m_pitch.clamp(minPitch, maxPitch);

		m_transform.rotateOnAxis(glm::vec3(0.0f, 1.0f, 0.0f), yawRotation, trs::TransformSpace::World);
		m_transform.rotateOnAxis(glm::vec3(1.0f, 0.0f, 0.0f), pitchRotation, trs::TransformSpace::Local);
	}

	void FPCamera::lookAt(const glm::vec3& target)
	{
		m_transform.lookAt(target);
	}

	glm::mat4 FPCamera::getViewMatrix() const
	{
		auto position = m_transform.getPosition();
		auto forward = m_transform.forward();
		auto view = position + forward;
		auto up = m_transform.up();

		return glm::lookAt(position, view, up);
	}
}  // namespace trc