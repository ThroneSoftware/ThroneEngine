#include <Standard/ManagerFactory.h>

#include <catch.hpp>

namespace Tests
{
	SCENARIO("Test make function")
	{
		GIVEN("A ManagerFactory")
		{
			std::unique_ptr<trs::IManagerFactory> managerFactory = std::make_unique<trs::ManagerFactory<int>>();
			WHEN("Making a Manager")
			{
				std::unique_ptr<trs::BaseManager> manager = managerFactory->make();
				THEN("The correct manager is returned")
				{
					REQUIRE(manager.get() != nullptr);
					REQUIRE(dynamic_cast<trs::Manager<int>*>(manager.get()) != nullptr);
				}
			}
		}
	}
}  // namespace Tests