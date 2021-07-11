#include "Transform.h"

namespace trc
{
	void Transform::lookAt(const Transform& target)
	{
		///

		resetCachedTransform();
	}

	void Transform::lookAt(const glm::vec3& target)
	{
		///

		resetCachedTransform();
	}

	void Transform::rotate(const glm::quat& rotation, TransformSpace space)
	{
		///

		resetCachedTransform();
	}

	void Transform::rotateOnAxis(const glm::vec3& axis, Radian angle, TransformSpace space)
	{
		///

		resetCachedTransform();
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
		///

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
			case trc::ScalingMode::Additive:
			{
				m_scale += scale;
				break;
			}
			case trc::ScalingMode::Multiplicative:
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
		///

		return glm::mat4();
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
}  // namespace trc