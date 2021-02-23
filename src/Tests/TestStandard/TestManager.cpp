#include "Standard/Manager.h"

#include <Tests/ProxyGmock.h>

#include <catch.hpp>

namespace Tests
{
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

		trs::NotifiedPtr<int> find(const trs::Manager<int>& manager, int value)
		{
			return manager.findIf([value](int& managerValue) {
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

	SCENARIO("Erase object")
	{
		GIVEN("A manager with many objects")
		{
			class MockObject
			{
			public:
				MockObject(std::size_t id)
				  : m_id(id)
				{
				}

				~MockObject()
				{
					dtor();
				}


				std::size_t m_id;

				MOCK_METHOD0(dtor, void());
			};

			auto manager = std::make_unique<trs::Manager<MockObject>>();
			manager->emplace(0);
			manager->emplace(1);
			manager->emplace(2);

			auto find = [](MockObject& obj) {
				return obj.m_id == 1;
			};


			auto ptr = manager->findIf(find);

			EXPECT_CALL(*ptr, dtor()).Times(1);


			WHEN("An object is erased")
			{
				manager->erase(*ptr);

				THEN("The object is removed from the manager")
				{
					auto actual = manager->findIf(find);
					REQUIRE(actual.getPtr() == nullptr);
					REQUIRE(manager->size() == 2);

					REQUIRE(ptr.getPtr() == nullptr);
				}
			}
		}
	}

	SCENARIO("Object type_index")
	{
		GIVEN("An int Manager")
		{
			trs::Manager<int> manager;
			WHEN("Setting the objectTypeIndex of BaseManager")
			{
				// Done by the Manager's constructor.
				THEN("The objectTypeIndex of BaseManager is set")
				{
					REQUIRE(manager.getObjectTypeIndex() == typeid(int));
				}
			}
		}
	}

	SCENARIO("Process function")
	{
		class MockObject
		{
		public:
			MockObject(std::size_t id)
			  : m_id(id)
			{
			}

			MOCK_METHOD0(func, void());

			std::size_t m_id = 0;
		};

		GIVEN("A manager with objects")
		{
			trs::Manager<MockObject> manager;
			std::vector<trs::SharedPtr<MockObject>> objectPtrs;

			for(size_t i = 0; i < 3; ++i)
			{
				manager.emplace(i);
				auto ptr = manager.findIf<trs::SharedPtr<MockObject>>([i](const MockObject& value) {
					return value.m_id == i;
				});
				objectPtrs.emplace_back(ptr);
				EXPECT_CALL(*ptr, func()).Times(1);
			}


			WHEN("Calling process on the manager")
			{
				manager.process([](MockObject& value) {
					value.func();
				});

				THEN("Every object's func is called")
				{
					for(auto& ptr: objectPtrs)
					{
						testing::Mock::VerifyAndClearExpectations(&*ptr);
					}
				}
			}
		}
	}
}  // namespace Tests