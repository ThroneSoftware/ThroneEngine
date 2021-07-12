#include "FPCamera.h"

namespace trc
{
	FPCamera::FPCamera(trs::Transform& transform)
	  : m_transform(transform)
	{
	}

	void FPCamera::rotate(glm::vec2 rotation)
	{
		m_transform.rotateOnAxis(glm::vec3(0.0f, 1.0f, 0.0f), trs::Degree(-rotation.x), trs::TransformSpace::World);
		m_transform.rotateOnAxis(glm::vec3(1.0f, 0.0f, 0.0f), trs::Degree(rotation.y), trs::TransformSpace::Local);
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