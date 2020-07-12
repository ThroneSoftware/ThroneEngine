#include <Core/System.h>
#include <Standard/Manager.h>
#include <Tests/ProxyGmock.h>

#include <catch.hpp>

namespace Tests
{
	class ComponentWithEvents
	{
	public:
		ComponentWithEvents(std::size_t id)
		  : m_id(id)
		{
		}

		MOCK_METHOD0(update, void());
		MOCK_METHOD0(lateUpdate, void());

		std::size_t m_id;
	};

	class ComponentWithoutEvents
	{
	public:
	};

	SCENARIO("Test Has<Function> System concepts")
	{
		REQUIRE(trc::SystemPrivate::HasUpdate<ComponentWithEvents>);
		REQUIRE(trc::SystemPrivate::HasLateUpdate<ComponentWithEvents>);

		REQUIRE(trc::SystemPrivate::HasUpdate<ComponentWithoutEvents> == false);
		REQUIRE(trc::SystemPrivate::HasLateUpdate<ComponentWithoutEvents> == false);
	}

	SCENARIO("Component with event functions")
	{
		GIVEN("A system of ComponentWithEvents")
		{
			trs::Manager<ComponentWithEvents> manager;
			trc::System<ComponentWithEvents> system(manager);

			std::vector<trs::SharedPtr<ComponentWithEvents>> objectPtrs;

			for(size_t i = 0; i < 3; ++i)
			{
				manager.emplace(i);
				auto ptr = manager.findIf([i](const ComponentWithEvents& value) {
					return value.m_id == i;
				});
				objectPtrs.emplace_back(ptr);
			}

			WHEN("Calling the update function")
			{
				for(auto& ptr: objectPtrs)
				{
					EXPECT_CALL(*ptr, update()).Times(1);
				}

				system.update();
				THEN("The update function is called")
				{
					for(auto& ptr: objectPtrs)
					{
						testing::Mock::VerifyAndClearExpectations(&*ptr);
					}
				}
			}

			WHEN("Calling the lateUpdate function")
			{
				for(auto& ptr: objectPtrs)
				{
					EXPECT_CALL(*ptr, lateUpdate()).Times(1);
				}

				system.lateUpdate();
				THEN("The lateUpdate function is called")
				{
					for(auto& ptr: objectPtrs)
					{
						testing::Mock::VerifyAndClearExpectations(&*ptr);
					}
				}
			}
		}
	}

	SCENARIO("Component without event functions")
	{
		GIVEN("A system of ComponentWithoutEvents")
		{
			trs::Manager<ComponentWithoutEvents> manager;
			trc::System<ComponentWithoutEvents> system(manager);

			std::vector<trs::SharedPtr<ComponentWithoutEvents>> objectPtrs;

			for(size_t i = 0; i < 3; ++i)
			{
				manager.emplace();
			}

			WHEN("Calling the update function")
			{
				system.update();
				THEN("Nothing happens")
				{
					// This test is mostly make sure it compiles.
				}
			}

			WHEN("Calling the lateUpdate function")
			{
				system.lateUpdate();
				THEN("Nothing happens")
				{
					// This test is mostly make sure it compiles.
				}
			}
		}
	}
}  // namespace Tests