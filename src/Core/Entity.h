#pragma once

#include <Standard/Pointers.h>

#include <vector>

namespace trc
{
	class Component;

	class Entity
	{
	public:

		const std::string& getId();

		void setParent(trs::SharedPtr<Entity> parent);
		void addChild(trs::SharedPtr<Entity> child);
		void removeChild(trs::SharedPtr<Entity> child);
		void removeChild(const std::string& id);

	private:
		std::string m_id;

		trs::SharedPtr<Entity> m_parent;
		std::vector<trs::SharedPtr<Entity>> m_children;

		std::vector<trs::SharedPtr<Component>> m_components;
	};
}  // namespace trc