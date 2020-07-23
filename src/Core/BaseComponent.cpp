#include "BaseComponent.h"

namespace trc
{
	BaseComponent::BaseComponent(Entity& entity)
	  : m_entity(entity)
	{
	}

	Entity& BaseComponent::getEntity()
	{
		return m_entity;
	}
}  // namespace trc