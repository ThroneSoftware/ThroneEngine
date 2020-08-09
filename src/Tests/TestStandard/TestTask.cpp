#include <Standard/Task.h>
#include <Tests/ProxyGmock.h>

#include <catch.hpp>

#include <execution>

namespace Tests
{
	class Counter
	{
	public:
		int operator++()
		{
			int c = counter;
			REQUIRE(c == 0);
			return ++counter;
		}

		int operator--()
		{
			int c = counter;
			REQUIRE(c == 1);
			return --counter;
		}

	private:
		std::atomic_int counter = 0;
	};

	SCENARIO("Test process")
	{
		GIVEN("Multiple tasks with dependencies on each other")
		{
			std::unique_ptr<trs::Task<int>> task1 = std::make_unique<trs::Task<int>>(std::make_unique<int>(1));

			auto depTask2 = std::vector({std::ref(*task1)});
			std::unique_ptr<trs::Task<int>> task2 = std::make_unique<trs::Task<int>>(std::make_unique<int>(2), depTask2);

			auto depTask3 = std::vector({std::ref(*task2)});
			std::unique_ptr<trs::Task<int>> task3 = std::make_unique<trs::Task<int>>(std::make_unique<int>(3), depTask3);

			std::list<trs::Task<int>*> tasks = {&*task1, &*task2, &*task3};

			Counter counter;

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
					// The "Then" is in class Counter
				}
				AND_THEN("The tasks were processed the correct order")
				{
					REQUIRE(taskOrder == std::vector{1, 2, 3});
				}
			}
		}
	}
}  // namespace Tests