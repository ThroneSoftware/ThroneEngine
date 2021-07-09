#pragma once

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
		void lookAt(Transform);
		void lookAt();

		void rotate();
		void rotateOnAxis();
		void getRotation();
		void setRotation();

		void translate();
		void getPosition();
		void setPosition();

		void scale();
		void getScale();
		void setScale();

		void right();
		void up();
		void forward();

	private:

		// children
	};
}  // namespace trc