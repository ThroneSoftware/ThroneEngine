#pragma once

#include <Standard/BaseManager.h>
#include <Standard/Manager.h>
#include <Utilities/Utility.h>

#include <memory>

namespace trc
{
	class ComponentTypeTrait;

	class ManagerList
	{
	public:
		const std::vector<std::reference_wrapper<const ComponentTypeTrait>>& getComponentTypeTraits();

		template <typename ComponentType>
		trs::Manager<ComponentType>& findManager()
		{
			auto found = std::find_if(m_managers.begin(), m_managers.end(), [](const std::unique_ptr<trs::BaseManager>& manager) {
				return manager->getObjectTypeIndex() == typeid(ComponentType);
			});

			if(found != m_managers.end())
			{
				return tru::staticDownCast<trs::Manager<ComponentType>&>(**found);
			}
			else
			{
				m_componentTypeTraits.emplace_back(ComponentType::getComponentTypeTrait());
				auto& newManager = m_managers.emplace_back(ComponentType::getComponentTypeTrait().makeManager());
				return tru::staticDownCast<trs::Manager<ComponentType>&>(*newManager);
			}
		}

	private:
		std::vector<std::reference_wrapper<const ComponentTypeTrait>> m_componentTypeTraits;
		std::vector<std::unique_ptr<trs::BaseManager>> m_managers;
	};

}  // namespace trc