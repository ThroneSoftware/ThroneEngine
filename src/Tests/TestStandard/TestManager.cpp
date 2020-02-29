#define TEST_MANAGER
#include "Standard/Manager.h"

#include <catch.hpp>
#include <gmock.h>

namespace Tests
{
	// Should we add the destruction mode?

	SCENARIO("Emplace object")
	{
		GIVEN("A manager")
		{
			WHEN("Emplacing an object")
			{
				THEN("The object is emplaced")
				{
				}
			}
		}
	}

	SCENARIO("Remove object")
	{
		GIVEN("A manager with one object")
		{
			WHEN("The last SharedPtr to the object is destroyed")
			{
				THEN("The object is removed from the manager")
				{
				}
			}
		}

		GIVEN("A manager with with many objects")
		{
			GIVEN("A SharedPtr of an object in the middle of the array")
			{
				WHEN("The last SharedPtr is destroyed")
				{
					THEN("The object is removed from the manager")
					{
						throw 1;
					}
				}
			}

			GIVEN("A SharedPtr of an object in the beggining of the array")
			{
				WHEN("The last SharedPtr is destroyed")
				{
					THEN("The object is removed from the manager")
					{
					}
				}
			}

			GIVEN("A SharedPtr of an object in the end of the array")
			{
				WHEN("The last SharedPtr is destroyed")
				{
					THEN("The object is removed from the manager")
					{
					}
				}
			}
		}
	}

}  // namespace Tests