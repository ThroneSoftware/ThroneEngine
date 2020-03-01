#define TEST_MANAGER
#include "Standard/Manager.h"

#include <catch.hpp>
#include <gmock.h>

namespace Tests
{
	// Should we add the destruction mode?

	namespace TestManagerPrivate
	{
		std::unique_ptr<trs::Manager<int>> makeManager(int objectCount)
		{
			auto manager = std::make_unique<trs::Manager<int>>();

			for(size_t i = 0; i < objectCount; ++i)
			{
				manager->emplace(static_cast<int>(i));
			}

			return manager;
		}

		trs::SharedPtr<int> find(const trs::Manager<int>& manager, int value)
		{
			return manager.find([value](int& managerValue) {
				return managerValue == value;
			});
		}
	}  // namespace TestManagerPrivate

	SCENARIO("Emplace object")
	{
		GIVEN("A manager")
		{
			trs::Manager<int> manager;
			WHEN("Emplacing an object")
			{
				constexpr int value = 10;
				manager.emplace(value);
				THEN("The object is emplaced")
				{
					auto ptr = TestManagerPrivate::find(manager, value);
					REQUIRE(ptr.getPtr() != nullptr);
					REQUIRE(*ptr.getPtr() == value);
					REQUIRE(manager.size() == 1);
				}
			}
		}

		GIVEN("A manager with multiple objects")
		{
			constexpr size_t size = 5;
			auto manager = TestManagerPrivate::makeManager(size);
			WHEN("Emplacing a new object")
			{
				constexpr int value = 10;
				manager->emplace(value);
				THEN("The object is emplaced")
				{
					auto ptr = TestManagerPrivate::find(*manager, value);
					REQUIRE(ptr.getPtr() != nullptr);
					REQUIRE(*ptr.getPtr() == value);
				}
				AND_THEN("Previously existing objects have not been corrupted")
				{
					REQUIRE(manager->size() == size + 1);
					for(size_t i = 0; i < size; ++i)
					{
						auto ptr = TestManagerPrivate::find(*manager, static_cast<int>(i));
						REQUIRE(ptr.getPtr() != nullptr);
					}
				}
			}
		}
	}

	SCENARIO("Remove object")
	{
		GIVEN("A manager with one object")
		{
			auto manager = TestManagerPrivate::makeManager(1);
			WHEN("The last SharedPtr to the object is destroyed")
			{
				constexpr int value = 0;
				auto ptr = TestManagerPrivate::find(*manager, value);
				ptr.reset();
				THEN("The object is removed from the manager")
				{
					auto actual = TestManagerPrivate::find(*manager, value);
					REQUIRE(actual.getPtr() == nullptr);
					REQUIRE(manager->size() == 0);
				}
			}
		}

		GIVEN("A manager with with many objects")
		{
			constexpr size_t size = 5;
			auto manager = TestManagerPrivate::makeManager(size);

			AND_GIVEN("A SharedPtr of an object at the end of the array")
			{
				constexpr int value = size - 1;
				auto ptr = TestManagerPrivate::find(*manager, value);
				WHEN("The last SharedPtr is destroyed")
				{
					ptr.reset();
					THEN("The object is removed from the manager")
					{
						auto actual = TestManagerPrivate::find(*manager, value);
						REQUIRE(actual.getPtr() == nullptr);
						REQUIRE(manager->size() == size - 1);
					}
				}
			}

			AND_GIVEN("A SharedPtr of an object at the beggining of the array")
			{
				constexpr int value = 0;
				auto ptr = TestManagerPrivate::find(*manager, value);
				WHEN("The last SharedPtr is destroyed")
				{
					ptr.reset();
					THEN("The object is removed from the manager")
					{
						auto actual = TestManagerPrivate::find(*manager, value);
						REQUIRE(actual.getPtr() == nullptr);
						REQUIRE(manager->size() == size - 1);
					}
				}
			}

			//AND_GIVEN("A SharedPtr of an object in the middle of the array")
			//{
			//	constexpr int value = size / 2;
			//	auto ptr = TestManagerPrivate::find(*manager, value);
			//	WHEN("The last SharedPtr is destroyed")
			//	{
			//		ptr.reset();
			//		THEN("The object is removed from the manager")
			//		{
			//			auto actual = TestManagerPrivate::find(*manager, value);
			//			REQUIRE(actual.getPtr() == nullptr);
			//			REQUIRE(manager->size() == size - 1);
			//		}
			//	}
			//}
		}
	}

}  // namespace Tests