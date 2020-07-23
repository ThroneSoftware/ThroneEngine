#include "SharedComponent.h"

#include "Component.h"
#include "Entity.h"

namespace trc
{
	SharedComponent::SharedComponent(trs::SharedPtr<Entity> topParent, trs::SharedPtr<BaseComponent> component)
	  : m_topParent(std::move(topParent))
	  , m_component(std::move(component))
	{
		connect();
	}

	SharedComponent::SharedComponent(const SharedComponent& other)
	  : m_topParent(other.m_topParent)
	  , m_component(other.m_component)
	{
		connect();
	}

	SharedComponent& SharedComponent::operator=(const SharedComponent& other)
	{
		m_topParent = other.m_topParent;
		m_component = other.m_component;
		connect();
		return *this;
	}

	SharedComponent::SharedComponent(SharedComponent&& other)
	  : m_topParent(std::move(other.m_topParent))
	  , m_component(std::move(other.m_component))
	{
		connect();
	}

	SharedComponent& SharedComponent::operator=(SharedComponent&& other)
	{
		m_topParent = std::move(other.m_topParent);
		m_component = std::move(other.m_component);
		connect();
		return *this;
	}

	void SharedComponent::connect()
	{
		m_topParentChangedSignalConnection = m_component->getEntity().connectToTopParentChanged([this](trs::SharedPtr<Entity> topParent) {
			m_topParent = std::move(topParent);
		});
	}
}  // namespace trc
