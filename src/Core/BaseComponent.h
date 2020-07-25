#pragma once

#include <typeinfo>

namespace trc
{
	class Entity;

	class BaseComponent
	{
		// Components are not virtual, even though they should be derived from.
		// Components are guaranteed to be destroyed with their full type so the derived class destructor will be called.
	public:
		BaseComponent(Entity& entity);

		Entity& getEntity();

	private:
		Entity& m_entity;
	};
}  // namespace trc