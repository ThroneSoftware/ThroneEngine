#include <Standard/Task.h>
#include <Tests/TestsUtilities/Utilities.h>

#include <catch.hpp>

#include <execution>

namespace Tests
{
	SCENARIO("Test process")
	{
		GIVEN("Multiple tasks with dependencies on each other")
		{
			trs::Task<int> task1 = trs::Task<int>(std::make_unique<int>(1));

			auto depTask2 = std::vector({std::ref(task1)});
			trs::Task<int> task2 = trs::Task<int>(std::make_unique<int>(2), depTask2);

			auto depTask3 = std::vector({std::ref(task2)});
			trs::Task<int> task3 = trs::Task<int>(std::make_unique<int>(3), depTask3);

			std::list<trs::Task<int>*> tasks = {&task1, &task2, &task3};

			UniqueCounter counter;

			for(auto& task: tasks)
			{
				task->connectToFinishedSignal(
					[&counter]() {
						--counter;
					},
					// The position is very important or else the counter can be incremented when it is 1 because the next task is already started.
					boost::signals2::connect_position::at_front);
			}

			WHEN("Processing them in parallel")
			{
				std::vector<int> taskOrder;
				std::for_each(std::execution::par, tasks.begin(), tasks.end(), [&counter, &taskOrder](trs::Task<int>* task) {
					task->process([&counter, &taskOrder](int val) {
						++counter;
						taskOrder.emplace_back(val);
					});
				});

				THEN("Only one task is processed at a time")
				{
					// The "Then" is in class UniqueCounter
				}
				AND_THEN("The tasks were processed the correct order")
				{
					REQUIRE(taskOrder == std::vector{1, 2, 3});
				}
			}
		}
	}
}  // namespace Tests