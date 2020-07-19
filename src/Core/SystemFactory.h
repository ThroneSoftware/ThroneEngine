#pragma once

#include "ISystemFactory.h"
#include "System.h"

namespace trc
{
	template <typename ComponentType>
	class SystemFactory : public ISystemFactory
	{
	public:
		std::unique_ptr<ISystem> make(ManagerList& managerList) const override
		{
			return std::make_unique<System<ComponentType>>(managerList.findManager<ComponentType>());
		}
	};

}  // namespace trc