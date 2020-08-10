#pragma once

#include <Standard/BaseManager.h>
#include <Standard/Manager.h>
#include <Utilities/Utility.h>

#include <memory>

namespace Tests
{
	// For unit tests purposes
	class TestManagerList;
}  // namespace Tests

namespace trc
{
	namespace ManagerListPrivate
	{
		template <typename T>
		concept HasGetComponentTypeTrait = requires()
		{
			T::getComponentTypeTrait();
		};
	}  // namespace ManagerListPrivate

	class ComponentTypeTrait;

	class ManagerList
	{
		// For unit tests purposes
		friend class Tests::TestManagerList;

	public:
		std::optional<std::reference_wrapper<const ComponentTypeTrait>> getComponentTypeTrait(std::type_index componentTypeIndex);
		const std::vector<std::reference_wrapper<const ComponentTypeTrait>>& getComponentTypeTraits() const;


		template <typename ComponentType>
		trs::Manager<ComponentType>& findManager() requires ManagerListPrivate::HasGetComponentTypeTrait<ComponentType>
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