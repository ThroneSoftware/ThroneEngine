#pragma once

#include "IManagerFactory.h"
#include "Manager.h"

namespace trs
{
	template <typename ObjectType>
	class ManagerFactory : public IManagerFactory
	{
	public:
		std::unique_ptr<BaseManager> make() const override
		{
			return std::make_unique<Manager<ObjectType>>();
		}
	};
}  // namespace trs