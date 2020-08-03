#pragma once

#include <boost/signals2.hpp>
#include <gsl/gsl>

#include <atomic>
#include <cassert>
#include <mutex>
#include <vector>

namespace trs::Private
{
	template <typename T>
	concept HasFinishedSignal = requires(T t)
	{
		t.connectToFinishedSignal(std::function<void()>());
	};

	// clang-format off
	template <typename T> requires HasFinishedSignal<T>
	class TaskFence
	// clang-format on
	{
	public:
		using value_type = T;

	public:
		TaskFence(const std::vector<std::reference_wrapper<value_type>>& dependencies = {})
		  : m_expected(gsl::narrow<int>(dependencies.size()))
		  , m_current(gsl::narrow<int>(dependencies.size()))
		{
			m_finishedSignalsConnections.resize(gsl::narrow<int>(dependencies.size()));
			for(auto& dependency: dependencies)
			{
				m_finishedSignalsConnections.emplace_back(dependency.get().connectToFinishedSignal([this]() {
					assert(m_current > 0);

					if(--m_current == 0)
					{
						m_cv.notify_one();
					}
				}));
			}
		}

		TaskFence(const TaskFence& other) = delete;
		TaskFence& operator=(const TaskFence& other) = delete;

		TaskFence(TaskFence&& other) = delete;
		TaskFence& operator=(TaskFence&& other) = delete;

		~TaskFence() = default;

		void wait() const
		{
			if(m_expected > 0)
			{
				std::unique_lock lock(m_mut);
				m_cv.wait(lock, [this]() {
					assert(m_current >= 0);
					return m_current == 0;
				});
			}
		}

		void reset() noexcept
		{
			m_current = m_expected;
		}

		int getDependencyCount() const
		{
			return m_expected;
		}

	private:
		mutable std::mutex m_mut;
		mutable std::condition_variable m_cv;
		const int m_expected = 0;
		std::atomic_int m_current = 0;

		std::vector<boost::signals2::scoped_connection> m_finishedSignalsConnections;
	};
}  // namespace trs::Private