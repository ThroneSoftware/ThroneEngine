#pragma once

#include "BaseComponent.h"
#include "ComponentTypeTraits/Dependencies.h"
#include "SystemFactory.h"

#include <Standard/ManagerFactory.h>

#include <memory>

namespace trc
{
	template <typename ComponentType, typename DependenciesTypeListT = DependenciesTypeList<>>
	class Component : public BaseComponent
	{
	public:
		Component(Entity& entity)
		  : BaseComponent(entity)
		{
		}

		static const ComponentTypeTrait& getComponentTypeTrait()
		{
			static const auto trait = ComponentTypeTrait(typeid(ComponentType),
														 Dependencies(DependenciesTypeListT::getTypeIndices()),
														 std::make_unique<trs::ManagerFactory<ComponentType>>(),
														 std::make_unique<SystemFactory<ComponentType>>());
			return trait;
		}
	};
}  // namespace trc