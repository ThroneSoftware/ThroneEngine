#pragma once

#include <typeinfo>

namespace trc
{
	class Entity;

	class Component
	{
		// Components are not virtual, even though they should be derived from.
		// Components are guaranteed to be destroyed with their full type so the derived class destructor will be called.
	public:
		template <typename ComponentType>
		Component(ComponentType&)
		  : m_typeId(typeid(ComponentType).hash_code())
		{
		}

	private:
		std::size_t m_typeId;
	};
}  // namespace trc