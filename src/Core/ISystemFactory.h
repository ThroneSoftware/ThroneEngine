#pragma once

#include "ISystem.h"
#include "ManagerList.h"

namespace trc
{
	class ISystemFactory
	{
	public:
		virtual ~ISystemFactory() = default;

		virtual std::unique_ptr<ISystem> make(ManagerList& managerList) const = 0;
	};
}  // namespace trc