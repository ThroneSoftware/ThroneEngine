#include <Core/Component.h>
#include <Core/Entity.h>
#include <Core/EventProcessor.h>
#include <Tests/ProxyGmock.h>
#include <Tests/TestsUtilities/Utilities.h>

#include <catch.hpp>

namespace Tests
{
	// Test all tasks are there
	// Test tasks have correct dependencies
	// Test with different order in ManagerList (dependency is not the first one in the list)

	template <typename ComponentType, typename DependenciesTypeListT = trc::DependenciesTypeList<>>
	class C : public trc::Component<ComponentType, DependenciesTypeListT>
	{
	public:
		C(trc::Entity& entity)
		  : trc::Component<ComponentType, DependenciesTypeListT>(entity)
		{
		}

		MOCK_METHOD0(update, void());
	};

	class C1 : public C<C1>
	{
	public:
		using C::C;
	};

	class C2 : public C<C2, trc::DependenciesTypeList<C1>>
	{
	public:
		using C::C;
	};

	class C3 : public C<C3, trc::DependenciesTypeList<C3>>
	{
	public:
		using C::C;
	};

	SCENARIO("Test loop function")
	{
		GIVEN("A manager list with component that have dependencies")
		{
			trc::ManagerList managerList;

			trc::Entity entity;

			managerList.findManager<C1>().emplace(entity);
			managerList.findManager<C2>().emplace(entity);
			managerList.findManager<C3>().emplace(entity);

			auto c1Ptr = managerList.findManager<C1>().findIf([&entity](C1& c1) {
				return &c1.getEntity() == &entity;
			});
			auto c2Ptr = managerList.findManager<C2>().findIf([&entity](C2& c2) {
				return &c2.getEntity() == &entity;
			});
			auto c3Ptr = managerList.findManager<C3>().findIf([&entity](C3& c3) {
				return &c3.getEntity() == &entity;
			});

			UniqueCounter counter;
			std::vector<int> order;

			ON_CALL(*c1Ptr, update()).WillByDefault([&order, &counter]() {
				++counter;
				order.emplace_back(1);
			});

			ON_CALL(*c2Ptr, update()).WillByDefault([&order, &counter]() {
				++counter;
				order.emplace_back(2);
			});

			ON_CALL(*c3Ptr, update()).WillByDefault([&order, &counter]() {
				++counter;
				order.emplace_back(3);
			});


			AND_GIVEN("An EventProcessor that uses the ManagerList")
			{
				trc::EventProcessor eventProcessor(managerList);

				WHEN("Looping with the EventProcessor")
				{
					eventProcessor.loop();

					THEN("Only one Component is processed at a time")
					{
						// The "Then" is in class UniqueCounter
					}
					AND_THEN("The components are updated in the correct order")
					{
						REQUIRE(order == std::vector({1, 2, 3}));
					}
				}
			}
		}
	}
}  // namespace Tests