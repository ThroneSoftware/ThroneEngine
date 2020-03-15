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
	}

	void Entity::addChild(const trs::SharedPtr<Entity>& child)
	{
		addChild(trs::SharedPtr<Entity>(child));
	}

	void Entity::addChild(trs::SharedPtr<Entity>&& child)
	{
		assert(child != nullptr);
		child->setParent(makeSharedFromThis());
		m_children.emplace_back(std::move(child));
	}

	void Entity::removeChild(const trs::SharedPtr<Entity>& child)
	{
		removeChild(trs::SharedPtr<Entity>(child));
	}

	void Entity::removeChild(trs::SharedPtr<Entity>&& child)
	{
		assert(child != nullptr);
		removeChild(child.getPtr());
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