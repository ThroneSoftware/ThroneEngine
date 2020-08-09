#pragma once

#include <boost/signals2.hpp>
#include <gsl/gsl>

#include <atomic>
#include <cassert>
#include <mutex>
#include <vector>

namespace trs::Private
{
	class TaskFence
	{
	public:
		TaskFence() = default;

		template <typename T>
		TaskFence(const std::vector<std::reference_wrapper<T>>& dependencies)
		  : m_expected(gsl::narrow<int>(dependencies.size()))
		  , m_current(gsl::narrow<int>(dependencies.size()))
		{
			m_finishedSignalsConnections.reserve(gsl::narrow<int>(dependencies.size()));
			for(auto& dependency: dependencies)
			{
				m_finishedSignalsConnections.emplace_back(dependency.get().connectToFinishedSignal([this]() {
					// Keep a temporary val so we don't have to do another atomic operation.
					// It's also "safer" because notify_one is never going to be called two times.
					// If we did m_current == 0, it would be possible for two threads to enter the if.
					int val = 0;
					{
						std::scoped_lock lock(m_mut);
						assert(m_current > 0);
						val = --m_current;
					}

					if(val == 0)
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
					return m_current == 0;
				});
			}
		}

		void reset() noexcept
		{
			m_current = m_expected;
		}

		int getDependencyCount() const noexcept
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