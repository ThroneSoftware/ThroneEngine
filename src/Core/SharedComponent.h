#pragma once

#include <Standard/Pointers.h>

namespace trc
{
	class Entity;
	class Component;

	class SharedComponent
	{
	public:
	private:
		trs::SharedPtr<Entity> m_topParent;
		trs::SharedPtr<Component> m_component;
	};
}  // namespace trc