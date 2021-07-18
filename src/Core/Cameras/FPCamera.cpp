#include "FPCamera.h"

#include <fmt/format.h>

#include <iostream>

namespace trc
{
	FPCamera::FPCamera(trs::Transform& transform)
	  : m_transform(transform)
	{
	}

	void FPCamera::rotate(glm::vec2 rotation)
	{
		auto pitchRotation = trs::Degree(rotation.y);

		m_transform.rotateOnAxis(glm::vec3(0.0f, 1.0f, 0.0f), trs::Degree(-rotation.x), trs::TransformSpace::World);
		m_transform.rotateOnAxis(glm::vec3(1.0f, 0.0f, 0.0f), pitchRotation, trs::TransformSpace::Local);

		if(m_transform.up().y <= 0.0f)
		{
			m_transform.rotateOnAxis(glm::vec3(1.0f, 0.0f, 0.0f), -pitchRotation, trs::TransformSpace::Local);
		}
	}

	void FPCamera::lookAt(const glm::vec3& target)
	{
		m_transform.lookAt(target);
	}

	glm::mat4 FPCamera::getViewMatrix() const
	{
		auto position = m_transform.getPosition();
		auto view = position + m_transform.forward();
		auto up = m_transform.up();

		return glm::lookAt(position, view, up);
	}

	trs::Transform& FPCamera::getTransform()
	{
		return m_transform;
	}

	const trs::Transform& FPCamera::getTransform() const
	{
		return m_transform;
	}
}  // namespace trc