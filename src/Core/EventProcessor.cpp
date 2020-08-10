#include "EventProcessor.h"

#include "ComponentTypeTraits/ComponentTypeTrait.h"

namespace trc
{
	namespace EventProcessorPrivate
	{
		// declaration because of the circular dependency between addTask and getTaskDependencies
		trs::Task<ISystem>& addTask(std::list<trs::Task<ISystem>>& tasks,
									std::map<std::type_index, trs::Task<ISystem>&>& tasksTypes,
									const ComponentTypeTrait& componentTypeTrait,
									ManagerList& managerList);


		std::optional<std::reference_wrapper<trs::Task<ISystem>>> findTask(std::map<std::type_index, trs::Task<ISystem>&>& tasksTypes,
																		   std::type_index typeIndex)
		{
			auto found = std::find_if(tasksTypes.begin(),
									  tasksTypes.end(),
									  [typeIndex](const std::pair<std::type_index, trs::Task<ISystem>&>& values) {
										  return values.first == typeIndex;
									  });
			return found != tasksTypes.end() ? std::optional(std::ref(found->second)) : std::nullopt;
		}

		std::vector<std::reference_wrapper<trs::Task<ISystem>>>
			getTaskDependencies(std::list<trs::Task<ISystem>>& tasks,
								std::map<std::type_index, trs::Task<ISystem>&>& tasksTypes,
								const ComponentTypeTrait& componentTypeTrait,
								ManagerList& managerList)
		{
			std::vector<std::reference_wrapper<trs::Task<ISystem>>> dependencies;
			for(auto& componentTypeIndex: componentTypeTrait.getDependencies())
			{
				auto found = findTask(tasksTypes, componentTypeIndex);

				if(found.has_value())
				{
					dependencies.emplace_back(*found);
				}
				else
				{
					auto dependencyTypeTrait = managerList.getComponentTypeTrait(componentTypeIndex);

					assert(dependencyTypeTrait.has_value());

					dependencies.emplace_back(addTask(tasks, tasksTypes, *dependencyTypeTrait, managerList));
				}
			}

			return dependencies;
		}

		trs::Task<ISystem>& addTask(std::list<trs::Task<ISystem>>& tasks,
									std::map<std::type_index, trs::Task<ISystem>&>& tasksTypes,
									const ComponentTypeTrait& componentTypeTrait,
									ManagerList& managerList)
		{
			auto foundTask = findTask(tasksTypes, componentTypeTrait.getTypeIndex());
			if(!foundTask.has_value())
			{
				auto dependencies = getTaskDependencies(tasks, tasksTypes, componentTypeTrait, managerList);

				auto& task = tasks.emplace_back(componentTypeTrait.makeSystem(managerList), std::move(dependencies));
				tasksTypes.emplace(componentTypeTrait.getTypeIndex(), task);

				return task;
			}
			else
			{
				return foundTask->get();
			}
		}
	}  // namespace EventProcessorPrivate

	/*static*/ std::list<trs::Task<ISystem>> EventProcessor::makeTaskList(ManagerList& managerList)
	{
		std::list<trs::Task<ISystem>> tasks;
		std::map<std::type_index, trs::Task<ISystem>&> tasksTypes;
		for(const auto& componentTrait: managerList.getComponentTypeTraits())
		{
			EventProcessorPrivate::addTask(tasks, tasksTypes, componentTrait, managerList);
		}
		return tasks;
	}

	EventProcessor::EventProcessor(std::list<trs::Task<ISystem>>&& tasks)
	  : m_taskList(std::move(tasks))
	{
		m_taskList.sort([](const trs::Task<ISystem>& t1, const trs::Task<ISystem>& t2) {
			return t1.getDependencyCount() < t2.getDependencyCount();
		});
	}

	EventProcessor::EventProcessor(ManagerList& managerList)
	  : EventProcessor(makeTaskList(managerList))
	{
	}

	void EventProcessor::loop()
	{
		update();

		lateUpdate();
	}

	void EventProcessor::update()
	{
		process([](ISystem& system) {
			system.update();
		});
	}

	void EventProcessor::lateUpdate()
	{
		process([](ISystem& system) {
			system.lateUpdate();
		});
	}
}  // namespace trc