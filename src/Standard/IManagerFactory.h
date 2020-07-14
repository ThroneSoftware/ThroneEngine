#pragma once

#include "BaseManager.h"

#include <memory>

namespace trs
{
	class IManagerFactory
	{
	public:
		virtual ~IManagerFactory() = default;

		virtual std::unique_ptr<BaseManager> make() const = 0;
	};
}  // namespace trs