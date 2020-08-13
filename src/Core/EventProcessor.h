#pragma once

#include "ISystem.h"
#include "ManagerList.h"

#include <Standard/Task.h>

#include <algorithm>
#include <execution>
#include <list>

namespace trc
{
	class EventProcessor
	{
	public:
		static std::list<trs::Task<ISystem>> makeTaskList(ManagerList& managerList,
														  std::map<std::type_index, trs::Task<ISystem>&>& tasksTypes);

		static std::list<trs::Task<ISystem>> makeTaskList(ManagerList& managerList);

	public:
		EventProcessor(std::list<trs::Task<ISystem>>&& tasks);
		EventProcessor(ManagerList& managerList);

		void loop();

	private:
		void update();

		void lateUpdate();

		template <typename Func>
		void process(Func func)
		{
			std::for_each(std::execution::par, m_taskList.begin(), m_taskList.end(), [func](trs::Task<ISystem>& task) {
				task.process(func);
			});

			for(auto& task: m_taskList)
			{
				task.reset();
			}
		}

		std::list<trs::Task<ISystem>> m_taskList;
	};
}  // namespace trc