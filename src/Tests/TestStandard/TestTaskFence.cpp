#include <Standard/Private/TaskFence.h>
#include <Tests/ProxyGmock.h>

#include <boost/signals2.hpp>
#include <catch.hpp>

#include <future>

namespace Tests
{
	class MockTask
	{
	public:
		boost::signals2::connection connectToFinishedSignal(std::function<void()> func)
		{
			return m_finishSignal.connect(func);
		}

		void finish()
		{
			m_finishSignal();
		}

	private:
		boost::signals2::signal<void()> m_finishSignal;
	};


	SCENARIO("Test wait")
	{
		GIVEN("A TaskFence with dependencies")
		{
			MockTask task1;
			MockTask task2;
			MockTask task3;

			trs::Private::TaskFence<MockTask> fence(std::vector{ std::ref(task1), std::ref(task2), std::ref(task3) });
			WHEN("Waiting on the fence")
			{
				auto fut = std::async(std::launch::async, [&fence]() {
					fence.wait();
				});

				THEN("Every dependencies needs to finish before the fence finish waiting")
				{
					REQUIRE(fut.wait_for(std::chrono::seconds(0)) == std::future_status::timeout);
					task1.finish();
					REQUIRE(fut.wait_for(std::chrono::seconds(0)) == std::future_status::timeout);
					task2.finish();
					REQUIRE(fut.wait_for(std::chrono::seconds(0)) == std::future_status::timeout);
					task3.finish();

					REQUIRE(fut.wait_for(std::chrono::seconds(30)) == std::future_status::ready);
				}
			}
		}

		GIVEN("A TaskFence with no dependencies")
		{
			trs::Private::TaskFence<MockTask> fence;
			WHEN("Waiting on the fence")
			{
				auto fut = std::async(std::launch::async, [&fence]() {
					fence.wait();
				});
				THEN("Then the wait returns immediately.")
				{
					REQUIRE(fut.wait_for(std::chrono::seconds(1)) == std::future_status::ready);
				}
			}
		}
	}
}  // namespace Tests