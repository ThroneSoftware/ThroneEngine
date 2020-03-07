#pragma once

#include <Standard/Pointers.h>

#include <vector>

namespace trc
{
	class Component;

	class Entity
	{
	public:
	private:
		std::string m_id;

		std::vector<trs::SharedPtr<Component>> m_components;
		std::vector<trs::SharedPtr<Entity>> m_children;
	};
}  // namespace trc