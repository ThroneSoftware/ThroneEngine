#include <Utilities/Utility.h>

#include <catch.hpp>

#include <atomic>

namespace Tests
{
	SCENARIO("Test implementation with applyIf")
	{
		auto multiply = [](int value) {
			return value * 2;
		};

		auto notZero = [](int value) {
			return value != 0;
		};


		GIVEN("An atomic equal to 0")
		{
			std::atomic<int> atomic = 0;

			WHEN("applyIf multiply notZero")
			{
				bool result = tru::applyIf(atomic, multiply, notZero);

				THEN("applyIf returns false and the atomic is still equal to 0")
				{
					REQUIRE(result == false);
					REQUIRE(atomic == 0);
				}
			}
		}

		GIVEN("An atomic equal to 2")
		{
			std::atomic<int> atomic = 2;

			WHEN("applyIf multiply notZero")
			{
				bool result = tru::applyIf(atomic, multiply, notZero);

				THEN("applyIf returns true and the atomic is equal to 4")
				{
					REQUIRE(result == true);
					REQUIRE(atomic == 4);
				}
			}
		}

		GIVEN("An atomic equal to -1")
		{
			std::atomic<int> atomic = -1;

			WHEN("applyIf multiply notZero")
			{
				bool result = tru::applyIf(atomic, multiply, notZero);

				THEN("applyIf returns true is equal to -2")
				{
					REQUIRE(result == true);
					REQUIRE(atomic == -2);
				}
			}
		}
	}

	SCENARIO("Test incrementNotEqual")
	{
		GIVEN("An atomic equal to 0")
		{
			std::atomic<int> atomic = 0;

			WHEN("incrementNotEqual 0")
			{
				bool result = tru::incrementNotEqual<0>(atomic);

				THEN("incrementNotEqual returns false and the atomic is still to 0")
				{
					REQUIRE(result == false);
					REQUIRE(atomic == 0);
				}
			}

			WHEN("incrementNotEqual 1")
			{
				bool result = tru::incrementNotEqual<1>(atomic);

				THEN("incrementNotEqual returns true and the atomic is equal to 1")
				{
					REQUIRE(result == true);
					REQUIRE(atomic == 1);
				}
			}
		}

		GIVEN("An atomic equal to 1")
		{
			std::atomic<int> atomic = 1;

			WHEN("incrementNotEqual 0")
			{
				bool result = tru::incrementNotEqual<0>(atomic);

				THEN("incrementNotEqual returns true and the atomic is equal to 2")
				{
					REQUIRE(result == true);
					REQUIRE(atomic == 2);
				}
			}
		}

		GIVEN("An atomic equal to -1")
		{
			std::atomic<int> atomic = -1;

			WHEN("incrementNotEqual 0")
			{
				bool result = tru::incrementNotEqual<0>(atomic);

				THEN("incrementNotEqual returns true is equal to 0")
				{
					REQUIRE(result == true);
					REQUIRE(atomic == 0);
				}
			}
		}
	}

	SCENARIO("Test decrementEqual")
	{
		GIVEN("An atomic equal to 0")
		{
			std::atomic<int> atomic = 0;

			WHEN("decrementEqual 1")
			{
				bool result = tru::decrementEqual<1>(atomic);

				THEN("incrementNotEqual returns false and the atomic is equal to 0")
				{
					REQUIRE(result == false);
					REQUIRE(atomic == 0);
				}
			}
		}

		GIVEN("An atomic equal to 1")
		{
			std::atomic<int> atomic = 1;

			WHEN("decrementEqual 1")
			{
				bool result = tru::decrementEqual<1>(atomic);

				THEN("incrementNotEqual returns true and the atomic is equal to 0")
				{
					REQUIRE(result == true);
					REQUIRE(atomic == 0);
				}
			}
		}
	}
}  // namespace Tests