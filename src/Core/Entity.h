#pragma once

#include <Standard/Pointers.h>

#include <vector>

namespace trc
{
	class Component;

	class Entity : public trs::EnableSharedFromThis<Entity>
	{
	public:
		const std::string& getId();

		void setParent(std::optional<std::reference_wrapper<Entity>> parent);

		void addChild(Entity& child);

		void removeChild(Entity& child);

		trs::SharedPtr<Entity> getParent() const;
		const std::vector<trs::SharedPtr<Entity>>& getChildren() const;

	private:
		void removeChild(Entity* child);

		std::string m_id;

		std::optional<std::reference_wrapper<Entity>> m_parent;
		std::vector<trs::SharedPtr<Entity>> m_children;

		std::vector<trs::SharedPtr<Component>> m_components;
	};
}  // namespace trc