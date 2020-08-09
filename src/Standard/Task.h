#pragma once

#include "Private/TaskFence.h"

#include <boost/signals2.hpp>

#include <typeindex>

namespace trs
{
	template <typename T>
	class Task
	{
	public:
		using value_type = T;

	public:
		Task(std::unique_ptr<value_type> value, const std::vector<std::reference_wrapper<Task<value_type>>>& dependencies = {})
		  : m_value(std::move(value))
		  , m_fence(dependencies)
		{
		}

		Task(const Task& other) = delete;
		Task& operator=(const Task& other) = delete;

		Task(Task&& other) = delete;
		Task& operator=(Task&& other) = delete;

		~Task() = default;

		template <typename Func>
		void process(Func func)
		{
			m_fence.wait();

			func(*m_value);

			m_finishedSignal();
		}

		template <typename Func>
		boost::signals2::connection
			connectToFinishedSignal(Func&& func, boost::signals2::connect_position position = boost::signals2::connect_position::at_back)
		{
			return m_finishedSignal.connect(std::forward<Func>(func), position);
		}

		void reset() noexcept
		{
			m_fence.reset();
		}

		int getDependencyCount() const noexcept
		{
			return m_fence.getDependencyCount();
		}

	private:
		Private::TaskFence m_fence;
		std::unique_ptr<value_type> m_value;
		boost::signals2::signal<void()> m_finishedSignal;
	};
}  // namespace trs