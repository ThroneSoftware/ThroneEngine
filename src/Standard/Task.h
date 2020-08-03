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
		Task(std::type_index componentTypeIndex,
			 std::unique_ptr<value_type> value,
			 const std::vector<std::reference_wrapper<Task<value_type>>>& dependencies = {})
		  : m_componentTypeIndex(componentTypeIndex)
		  , m_value(std::move(value))
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
		boost::signals2::connection connectToFinishedSignal(Func&& func)
		{
			return m_finishedSignal.connect(std::forward<Func>(func));
		}

		void reset()
		{
			m_fence.reset();
		}

		std::type_index getComponentTypeIndex() const
		{
			return m_componentTypeIndex;
		}

		int getDependencyCount() const
		{
			return m_fence.getDependencyCount();
		}

	private:
		Private::TaskFence<Task<value_type>> m_fence;
		std::unique_ptr<value_type> m_value;
		boost::signals2::signal<void()> m_finishedSignal;

		std::type_index m_componentTypeIndex;
	};
}  // namespace trs