#include <Core/Component.h>
#include <Core/ComponentTypeTraits/ComponentTypeTrait.h>
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

	class C3 : public C<C3, trc::DependenciesTypeList<C2>>
	{
	public:
		using C::C;
	};

	class C4 : public C<C4, trc::DependenciesTypeList<C2, C3>>
	{
	public:
		using C::C;
	};

	SCENARIO("Test makeTaskList")
	{
		GIVEN("A ManagerList with an unordered list of components with dependencies on each other")
		{
			trc::ManagerList managerList;
			trc::Entity entity;

			// Keep this order to cover more use cases.
			managerList.findManager<C1>().emplace(entity);
			managerList.findManager<C3>().emplace(entity);
			managerList.findManager<C2>().emplace(entity);
			managerList.findManager<C4>().emplace(entity);

			WHEN("Making the task list")
			{
				std::map<std::type_index, trs::Task<trc::ISystem>&> tasksTypes;
				auto taskList = trc::EventProcessor::makeTaskList(managerList, tasksTypes);

				THEN("All the tasks have the correct number of dependencies")
				{
					for(auto& [taskType, task]: tasksTypes)
					{
						// Not very nice looking, but there wasn't a better solution when it was written.
						if(taskType == typeid(C1))
						{
							REQUIRE(task.getDependencyCount() == C1::getComponentTypeTrait().getDependencies().size());
						}
						else if(taskType == typeid(C2))
						{
							REQUIRE(task.getDependencyCount() == C2::getComponentTypeTrait().getDependencies().size());
						}
						else if(taskType == typeid(C3))
						{
							REQUIRE(task.getDependencyCount() == C3::getComponentTypeTrait().getDependencies().size());
						}
						else if(taskType == typeid(C4))
						{
							REQUIRE(task.getDependencyCount() == C4::getComponentTypeTrait().getDependencies().size());
						}
						else
						{
							// Invalid type, should not happen.
							REQUIRE(false);
						}
					}
				}
				AND_THEN("THe list is the correct size")
				{
					REQUIRE(taskList.size() == 4);
				}
			}
		}
	}

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

			EXPECT_CALL(*c1Ptr, update()).Times(1).WillOnce([&order, &counter]() {
				++counter;
				order.emplace_back(1);
				--counter;
			});
			EXPECT_CALL(*c2Ptr, update()).Times(1).WillOnce([&order, &counter]() {
				++counter;
				order.emplace_back(2);
				--counter;
			});
			EXPECT_CALL(*c3Ptr, update()).Times(1).WillOnce([&order, &counter]() {
				++counter;
				order.emplace_back(3);
				--counter;
			});

			AND_GIVEN("An EventProcessor that uses the ManagerList")
			{
				auto taskList = trc::EventProcessor::makeTaskList(managerList);

				REQUIRE(taskList.size() == 3);

				trc::EventProcessor eventProcessor(std::move(taskList));

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