#include "Entity.h"

namespace trc
{
	const std::string& Entity::getId()
	{
		return m_id;
	}

	void Entity::setParent(std::optional<std::reference_wrapper<Entity>> parent)
	{
		if(m_parent.has_value())
		{
			m_parent->get().removeChild(this);
		}
		m_parent = parent;
		if(m_parent.has_value())
		{
			m_parent->get().m_children.emplace_back(makeSharedFromThis());
		}
	}

	void Entity::addChild(const trs::SharedPtr<Entity>& child)
	{
		addChild(trs::SharedPtr<Entity>(child));
	}

	void Entity::addChild(trs::SharedPtr<Entity>&& child)
	{
		assert(child != nullptr);
		assert(&child->m_parent->get() != this);  // assert child is not already a child of this
		child->setParent(*this);
	}

	void Entity::removeChild(const trs::SharedPtr<Entity>& child)
	{
		assert(child != nullptr);
		removeChild(child.getPtr());
	}

	trs::SharedPtr<Entity> Entity::getParent() const
	{
		return m_parent->get().makeSharedFromThis();
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