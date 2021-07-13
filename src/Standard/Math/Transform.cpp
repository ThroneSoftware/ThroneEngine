#include "Transform.h"

namespace trs
{
	void Transform::lookAt(const Transform& target)
	{
		lookAt(target.m_position);
	}

	void Transform::lookAt(const glm::vec3& target)
	{
		rotate(getLookAtRotation(target), TransformSpace::World);
	}

	glm::quat Transform::getLookAtRotation(const glm::vec3& target) const
	{
		if(target == m_position)
		{
			return glm::identity<glm::quat>();
		}

		glm::vec3 vec1 = forward();
		glm::vec3 vec2 = glm::normalize(target - m_position);

		auto dot = Real(glm::dot(vec1, vec2));

		if(dot == 1.0f)	 // Same direction, no rotation needed
		{
			return glm::identity<glm::quat>();
		}
		else if(dot == -1.0f)  // Directly opposed direction
		{
			// Rotate 180 degree on the up axis
			// Valid because:
			// vec1 == forward
			// forward and up are perpendicular
			// vec1 and vec2 are antiparallel
			// vec1 and vec2 are perpendicular to up
			return glm::normalize(glm::angleAxis(**Radian(std::numbers::pi_v<float>), up()));
		}
		else
		{
			// cos = cos(theta / 2)
			auto cos = std::sqrt((dot + 1.0f) * 2.0f) / 2.0f;
			// sin = sin(theta / 2)
			auto sin = 1.0f / (cos * 2.0f);

			auto axis = glm::cross(vec1, vec2);

			return glm::normalize(glm::quat(cos, axis * sin));
		}
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