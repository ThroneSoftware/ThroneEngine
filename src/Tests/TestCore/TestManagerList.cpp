#include "MockComponent.h"

#include <Core/ManagerList.h>

#include <catch.hpp>

namespace Tests
{
	class TestManagerList
	{
	public:
		static decltype(trc::ManagerList::m_managers)& getManagers(trc::ManagerList& managerList)
		{
			return managerList.m_managers;
		}
	};

	SCENARIO("Test findManager")
	{
		GIVEN("An empty ManagerList")
		{
			trc::ManagerList managerList;
			WHEN("Finding a manager")
			{
				auto& manager = managerList.findManager<Mocks::MockComponent>();
				THEN("Manager is returned")
				{
					auto& managers = TestManagerList::getManagers(managerList);
					REQUIRE(managers.size() == 1);
					REQUIRE(&*managers[0] == &manager);
				}
			}
		}

		GIVEN("A ManagerList with a Manager")
		{
			trc::ManagerList managerList;
			managerList.findManager<Mocks::MockComponent>();
			REQUIRE(TestManagerList::getManagers(managerList).size() == 1);
			WHEN("When finding the Manager")
			{
				auto& manager = managerList.findManager<Mocks::MockComponent>();
				THEN("The Manager is returned")
				{
					auto& managers = TestManagerList::getManagers(managerList);
					REQUIRE(managers.size() == 1);
					REQUIRE(&*managers[0] == &manager);
				}
			}
		}
	}

	SCENARIO("Test get component traits")
	{
		GIVEN("A ManagerList with managers")
		{
			trc::ManagerList managerList;
			managerList.findManager<Mocks::MockComponent>();
			WHEN("When getting the component type traits")
			{
				const auto& componentTypeTraits = managerList.getComponentTypeTraits();
				THEN("The component type traits are returned")
				{
					REQUIRE(componentTypeTraits.size() == 1);
					REQUIRE(componentTypeTraits[0].get().getTypeIndex() == Mocks::MockComponent::getComponentTypeTrait().getTypeIndex());
				}
			}
		}
	}
}  // namespace Tests