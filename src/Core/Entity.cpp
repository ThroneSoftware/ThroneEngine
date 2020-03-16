#include "Entity.h"

namespace trc
{
	const std::string& Entity::getId()
	{
		return m_id;
	}

	void Entity::setParent(const trs::SharedPtr<Entity>& parent)
	{
		setParent(trs::SharedPtr<Entity>(parent));
	}

	void Entity::setParent(trs::SharedPtr<Entity>&& parent)
	{
		if(m_parent != nullptr)
		{
			m_parent->removeChild(this);
		}
		m_parent = std::move(parent);
		if(m_parent != nullptr)
		{
			m_parent->m_children.emplace_back(makeSharedFromThis());
		}
	}

	void Entity::addChild(const trs::SharedPtr<Entity>& child)
	{
		addChild(trs::SharedPtr<Entity>(child));
	}

	void Entity::addChild(trs::SharedPtr<Entity>&& child)
	{
		assert(child != nullptr);
		assert(child->m_parent.getPtr() != this);  // assert child is not already a child of this
		child->setParent(makeSharedFromThis());
	}

	void Entity::removeChild(const trs::SharedPtr<Entity>& child)
	{
		assert(child != nullptr);
		removeChild(child.getPtr());
	}

	trs::SharedPtr<Entity> Entity::getParent() const
	{
		return m_parent;
	}

	const std::vector<trs::SharedPtr<Entity>>& Entity::getChildren() const
	{
		return m_children;
	}

	void Entity::removeChild(Entity* child)
	{
		auto found = std::find_if(m_children.begin(), m_children.end(), [&child](const trs::SharedPtr<Entity>& value) {
			return value.getPtr() == child;
		});

		assert(found != m_children.end());

		m_children.erase(found);
	}
}  // namespace trc