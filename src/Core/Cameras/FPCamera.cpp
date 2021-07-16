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
		constexpr auto maxPitch = trs::Degree(69.5f);
		constexpr auto minPitch = trs::Degree(-69.5f);

		auto pitchRotation = trs::Degree(rotation.y);

		auto euler = glm::eulerAngles(m_transform.getRotation());
		//std::cout << fmt::format("Euler: pitch: {}, yaw: {}, roll: {}\n", euler.x, euler.y, euler.z);

		auto q = m_transform.getRotation();
		auto p1 = std::atan2((2.0f * q.x * q.w) + (2.0f * q.y * q.z), 1 - (2.0f * q.x * q.x) - (2.0f * q.z * q.z));
		auto p2 = std::atan2(1 - (2.0f * q.x * q.x) - (2.0f * q.z * q.z), (2.0f * q.x * q.w) + (2.0f * q.y * q.z));

		//std::cout << "p1: " << p1 << " p2: " << p2 << "\n";

		//auto pitch = pitchRotation + trs::Radian(p1);
		//if(pitch >= maxPitch)
		//{
		//	pitchRotation -= pitch - maxPitch;
		//}
		//else if(pitch <= minPitch)
		//{
		//	pitchRotation -= pitch - minPitch;
		//}

		m_transform.rotateOnAxis(glm::vec3(0.0f, 1.0f, 0.0f), trs::Degree(-rotation.x), trs::TransformSpace::World);
		m_transform.rotateOnAxis(glm::vec3(1.0f, 0.0f, 0.0f), pitchRotation, trs::TransformSpace::Local);

		if(m_transform.up().y <= 0.0f)
		{
			m_transform.rotateOnAxis(glm::vec3(1.0f, 0.0f, 0.0f), -pitchRotation, trs::TransformSpace::Local);
		}

		//std::cout << fmt::format("pitch: {}\n", fmt::to_string(pitch));
		//std::cout << fmt::format("pitchRotation: {}\n", fmt::to_string(pitchRotation));
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