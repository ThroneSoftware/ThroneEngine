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

		void setParent(const trs::SharedPtr<Entity>& parent);
		void setParent(trs::SharedPtr<Entity>&& parent);

		void addChild(const trs::SharedPtr<Entity>& child);
		void addChild(trs::SharedPtr<Entity>&& child);

		void removeChild(const trs::SharedPtr<Entity>& child);

		trs::SharedPtr<Entity> getParent() const;
		const std::vector<trs::SharedPtr<Entity>>& getChildren() const;

	private:
		void removeChild(Entity* child);

		std::string m_id;

		trs::SharedPtr<Entity> m_parent;
		std::vector<trs::SharedPtr<Entity>> m_children;

		std::vector<trs::SharedPtr<Component>> m_components;
	};
}  // namespace trc