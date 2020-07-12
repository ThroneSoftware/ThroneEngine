#include <Core/ComponentTypeTraits/Dependencies.h>
#include <Tests/ProxyGmock.h>

#include <catch.hpp>

namespace Tests
{
    SCENARIO("Test getDependencies")
    {
		GIVEN("A Dependencies with multiple dependencies")
		{
			trc::Dependencies dependencies({typeid(int), typeid(double)});
			WHEN("Getting the dependencies")
			{
				const auto& dep = dependencies.getDependencies();
				THEN("The dependencies are returned")
				{
					REQUIRE(dep.size() == 2);
					REQUIRE(dep[0] == typeid(int));
					REQUIRE(dep[1] == typeid(double));
				}
			}
		}
    }
}