#pragma once

#include "Angle.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <optional>

namespace trc
{
	enum class TransformSpace
	{
		Local,
		World,
		Parent
	};

	enum class ScalingMode
	{
		Additive,
		Multiplicative
	};

	class Transform
	{
	public:
		void lookAt(const Transform& target);
		void lookAt(const glm::vec3& target);
		glm::quat getLookAtRotation(const glm::vec3& target) const;

		void rotate(const glm::quat& rotation, TransformSpace space);
		void rotateOnAxis(const glm::vec3& axis, Radian angle, TransformSpace space);
		void setRotation(const glm::quat& rotation);
		glm::quat getRotation() const;

		void translate(const glm::vec3& translation, TransformSpace space);
		void setPosition(const glm::vec3& position);
		glm::vec3 getPosition() const;

		void scale(const glm::vec3& scale, ScalingMode mode);
		void setScale(const glm::vec3& scale);
		glm::vec3 getScale() const;

		glm::vec3 right() const;
		glm::vec3 up() const;
		glm::vec3 forward() const;

		void updateCachedTransform();
		glm::mat4 computeTransform() const;

		glm::mat4& getCachedTransformRef();

	private:
		void resetCachedTransform();

		// children

		std::optional<glm::mat4> m_cachedTransform;

		glm::quat m_rotation;
		glm::vec3 m_position;
		glm::vec3 m_scale;
	};

	//constexpr auto size = sizeof(Transform);
}  // namespace trc