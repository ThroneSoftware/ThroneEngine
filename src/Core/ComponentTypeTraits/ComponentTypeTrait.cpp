#include "ComponentTypeTrait.h"

namespace trc
{
	ComponentTypeTrait::ComponentTypeTrait(std::type_index typeIndex,
		Dependencies&& m_dependencies,
		std::unique_ptr<trs::IManagerFactory> managerFactory,
		std::unique_ptr<ISystemFactory> systemFactory)
		: m_typeIndex(typeIndex)
		, m_dependencies(std::move(m_dependencies))
		, m_managerFactory(std::move(managerFactory))
		, m_systemFactory(std::move(systemFactory))
	{
	}

	std::type_index ComponentTypeTrait::getTypeIndex() const
	{
		return m_typeIndex;
	}

	const std::vector<std::type_index>& ComponentTypeTrait::getDependencies() const
	{
		return m_dependencies.getDependencies();
	}

	std::unique_ptr<trs::BaseManager> ComponentTypeTrait::makeManager() const
	{
		return m_managerFactory->make();
	}

	std::unique_ptr<ISystem> ComponentTypeTrait::makeSystem(ManagerList& managerList) const
	{
		return m_systemFactory->make(managerList);
	}
}