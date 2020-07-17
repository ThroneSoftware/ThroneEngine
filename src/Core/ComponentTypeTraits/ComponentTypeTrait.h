#pragma once

#include "../ISystemFactory.h"
#include "Dependencies.h"

#include <Standard/IManagerFactory.h>

#include <typeindex>

namespace trc
{
	class ComponentTypeTrait
	{
	public:
		ComponentTypeTrait(std::type_index typeIndex,
						   Dependencies&& m_dependencies,
						   std::unique_ptr<trs::IManagerFactory> managerFactory,
						   std::unique_ptr<ISystemFactory> systemFactory);

		std::type_index getTypeIndex() const;

		const std::vector<std::type_index>& getDependencies() const;

		std::unique_ptr<trs::BaseManager> makeManager() const;

		std::unique_ptr<ISystem> makeSystem(ManagerList& managerList) const;

	private:
		const std::type_index m_typeIndex;

		Dependencies m_dependencies;

		std::unique_ptr<trs::IManagerFactory> m_managerFactory;
		std::unique_ptr<ISystemFactory> m_systemFactory;
	};
}  // namespace trc