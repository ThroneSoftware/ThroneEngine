#include "Transform.h"

#include <fmt/format.h>

#include <iostream>

namespace trs
{
	void Transform::lookAt(const Transform& target)
	{
		lookAt(target.m_position);
	}

	void Transform::lookAt(const glm::vec3& target)
	{
		setRotation(getLookAtRotation(target));
	}

	glm::quat Transform::getLookAtRotation(const glm::vec3& target) const
	{
		constexpr auto worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		constexpr auto worldForward = glm::vec3(0.0f, 0.0f, 1.0f);

		auto normTarget = glm::normalize(target - m_position);

		if(normTarget == glm::vec3(0.0f))
		{
			return m_rotation;
		}

		glm::vec3 perpendicularToForward = {};

		if(auto dot = glm::dot(normTarget, worldUp); dot != 1.0f && dot != -1.0f)
		{
			perpendicularToForward = worldUp;
		}
		else
		{
			perpendicularToForward = worldForward;
		}


		auto forward = normTarget;
		auto right = glm::normalize(glm::cross(perpendicularToForward, forward));
		auto up = glm::normalize(glm::cross(forward, right));

		glm::mat3 rotation = glm::mat3(right, up, forward);

		return glm::quat_cast(rotation);
	}

	void Transform::rotate(const glm::quat& rotation, TransformSpace space)
	{
		switch(space)
		{
			case TransformSpace::Local:
			{
				m_rotation = m_rotation * rotation;
				break;
			}
			case TransformSpace::World:
			{
				// why the fuck does this work
				m_rotation = m_rotation * glm::inverse(m_rotation) * rotation * m_rotation;
				break;
			}
			case TransformSpace::Parent:
			{
				// todo

				break;
			}
			default:
				break;
		}

		resetCachedTransform();
	}

	void Transform::rotateOnAxis(const glm::vec3& axis, Radian angle, TransformSpace space)
	{
		rotate(glm::angleAxis(**angle, axis), space);
	}

	void Transform::setRotation(const glm::quat& rotation)
	{
		m_rotation = rotation;

		resetCachedTransform();
	}

	glm::quat Transform::getRotation() const
	{
		return m_rotation;
	}

	void Transform::translate(const glm::vec3& translation, TransformSpace space)
	{
		switch(space)
		{
			case TransformSpace::Local:
			{
				m_position += m_rotation * translation;
				break;
			}
			case TransformSpace::World:
			{
				m_position += translation;
				break;
			}
			case TransformSpace::Parent:
			{
				// todo
				break;
			}
			default:
				break;
		}

		resetCachedTransform();
	}

	void Transform::setPosition(const glm::vec3& position)
	{
		m_position = position;

		resetCachedTransform();
	}

	glm::vec3 Transform::getPosition() const
	{
		return m_position;
	}

	void Transform::scale(const glm::vec3& scale, ScalingMode mode)
	{
		switch(mode)
		{
			case ScalingMode::Additive:
			{
				m_scale += scale;
				break;
			}
			case ScalingMode::Multiplicative:
			{
				m_scale *= scale;
				break;
			}
			default:
				break;
		}

		resetCachedTransform();
	}

	void Transform::setScale(const glm::vec3& scale)
	{
		m_scale = scale;

		resetCachedTransform();
	}

	glm::vec3 Transform::getScale() const
	{
		return m_scale;
	}

	glm::vec3 Transform::right() const
	{
		return m_rotation * glm::vec3(1.0f, 0.0f, 0.0f);
	}

	glm::vec3 Transform::up() const
	{
		return m_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
	}

	glm::vec3 Transform::forward() const
	{
		return m_rotation * glm::vec3(0.0f, 0.0f, 1.0f);
	}

	void Transform::updateCachedTransform()
	{
		m_cachedTransform.emplace(computeTransform());
	}

	glm::mat4 Transform::computeTransform() const
	{
		glm::mat4 transform = glm::identity<glm::mat4>();

		transform = glm::scale(transform, m_scale);
		transform = glm::mat4_cast(m_rotation) * transform;

		transform[3] = glm::vec4(m_position, 1.0f);

		return transform;
	}

	glm::mat4& Transform::getCachedTransformRef()
	{
		if(!m_cachedTransform)
		{
			updateCachedTransform();
		}
		return *m_cachedTransform;
	}

	void Transform::resetCachedTransform()
	{
		m_cachedTransform.reset();
	}
}  // namespace trs