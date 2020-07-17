#pragma once

#include <Core/ComponentTypeTraits/ComponentTypeTrait.h>
#include <Core/ComponentTypeTraits/Dependencies.h>
#include <Core/SystemFactory.h>
#include <Standard/ManagerFactory.h>

namespace Mocks
{
	class MockComponent
	{
	public:
		static const trc::ComponentTypeTrait& getComponentTypeTrait()
		{
			static const auto trait = trc::ComponentTypeTrait(typeid(MockComponent),
															  trc::Dependencies(),
															  std::make_unique<trs::ManagerFactory<MockComponent>>(),
															  std::make_unique<trc::SystemFactory<MockComponent>>());
			return trait;
		}
	};
}  // namespace Mocks