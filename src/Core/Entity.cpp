#include "Entity.h"

namespace trc
{
	namespace EntityPrivate
	{
		Entity& choseTopParent(Entity& parent)
		{
			const auto& parentData = parent.m_parentData;
			return parentData.has_value() ? parentData->m_topParent : parent;
		}
	}  // namespace EntityPrivate

	Entity::Parent::Parent(Entity& topParent, Entity& parent)
	  : m_topParent(topParent)
	  , m_parent(parent)
	{
	}

	const std::string& Entity::getId()
	{
		return m_id;
	}

	void Entity::setParent(std::optional<std::reference_wrapper<Entity>> parent)
	{
		if(m_parentData.has_value())
		{
			m_parentData->m_parent.removeChild(this);
		}

		if(parent.has_value())
		{
			m_parentData.emplace(EntityPrivate::choseTopParent(parent->get()), parent->get());
			m_parentData->m_parent.m_children.emplace_back(makeSharedFromThis());
			m_topParentChanged(m_parentData->m_topParent.makeSharedFromThis());
		}
		else
		{
			m_parentData = std::nullopt;
			m_topParentChanged(nullptr);
		}
	}

	void Entity::addChild(Entity& child)
	{
		// assert child is not already a child of this
		assert(&child.m_parentData->m_parent != this);
		child.setParent(*this);
	}

	void Entity::removeChild(Entity& child)
	{
		// assert child is a child of this
		assert(&child.m_parentData->m_parent == this);
		child.setParent(std::nullopt);
	}

	trs::SharedPtr<Entity> Entity::getTopParent() const
	{
		return m_parentData.has_value() ? m_parentData->m_topParent.makeSharedFromThis() : nullptr;
	}

	trs::SharedPtr<Entity> Entity::getParent() const
	{
		return m_parentData.has_value() ? m_parentData->m_parent.makeSharedFromThis() : nullptr;
	}

	const std::vector<trs::SharedPtr<Entity>>& Entity::getChildren() const
	{
		return m_children;
	}

	void Entity::removeChild(Entity* child)
	{
		auto found = std::find_if(m_children.begin(), m_children.end(), [child](const trs::SharedPtr<Entity>& value) {
			return value.getPtr() == child;
		});

		assert(found != m_children.end());

		m_children.erase(found);
	}
}  // namespace trc