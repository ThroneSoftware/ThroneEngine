#include "Entity.h"

namespace trc
{
	const std::string& Entity::getId()
	{
		return m_id;
	}

	void Entity::setParent(trs::SharedPtr<Entity> parent)
	{
		if(m_parent != nullptr)
		{
			m_parent->removeChild(getId());
		}
		m_parent = std::move(parent);
	}

	void Entity::addChild(trs::SharedPtr<Entity> child)
	{
		assert(child != nullptr);
		child->setParent();
		m_children.emplace_back(std::move(child));
	}

	void Entity::removeChild(trs::SharedPtr<Entity> child)
	{
		assert(child != nullptr);
		removeChild(child->getId());
	}

	// Should we implement the remove with a pointer instead?

	//void Entity::removeChild(const std::string& id)
	//{
	//	auto found = std::find_if(m_children.begin(), m_children.end(), [&id](const trs::SharedPtr<Entity>& child) {
	//		return child->getId() == id;
	//	});

	//	assert(found != m_children.end());

	//	m_children.erase(found);
	//}
}  // namespace trc