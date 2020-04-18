#pragma once

#include <Standard/Pointers.h>

#include <boost/signals2.hpp>

#include <vector>

namespace trc
{
	class Component;
	class Entity;

	namespace EntityPrivate
	{
		Entity& choseTopParent(Entity& parent);
	}

	class Entity : public trs::EnableSharedFromThis<Entity>
	{
		friend Entity& EntityPrivate::choseTopParent(Entity& parent);

		struct Parent
		{
			Parent(Entity& topParent, Entity& parent);

			Entity& m_topParent;
			Entity& m_parent;
		};

	public:
		const std::string& getId();

		void setParent(std::optional<std::reference_wrapper<Entity>> parent);

		void addChild(Entity& child);

		void removeChild(Entity& child);

		trs::SharedPtr<Entity> getTopParent() const;
		trs::SharedPtr<Entity> getParent() const;
		const std::vector<trs::SharedPtr<Entity>>& getChildren() const;

	private:
		void removeChild(Entity* child);

		std::string m_id;

		std::optional<Parent> m_parentData;
		boost::signals2::signal<void(trs::SharedPtr<Entity>)> m_topParentChanged;
		std::vector<trs::SharedPtr<Entity>> m_children;

		std::vector<trs::SharedPtr<Component>> m_components;
	};
}  // namespace trc