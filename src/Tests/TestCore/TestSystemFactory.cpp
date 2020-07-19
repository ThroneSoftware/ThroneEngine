#include "MockComponent.h"

#include <Core/ManagerList.h>
#include <Core/SystemFactory.h>
#include <Standard/ManagerFactory.h>

#include <catch.hpp>

namespace Tests
{
	SCENARIO("Test make function")
	{
		GIVEN("A SystemFactory")
		{
			trc::ManagerList managerList;
			std::unique_ptr<trc::ISystemFactory> systemFactory = std::make_unique<trc::SystemFactory<Mocks::MockComponent>>();
			WHEN("Making a System")
			{
				std::unique_ptr<trc::ISystem> system = systemFactory->make(managerList);
				THEN("The correct system is returned")
				{
					REQUIRE(system.get() != nullptr);
					REQUIRE(dynamic_cast<trc::System<Mocks::MockComponent>*>(system.get()) != nullptr);
				}
			}
		}
	}
}  // namespace Tests