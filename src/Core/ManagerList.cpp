#include "ManagerList.h"

#include "ComponentTypeTraits/ComponentTypeTrait.h"

#include <algorithm>

namespace trc
{
	std::optional<std::reference_wrapper<const ComponentTypeTrait>> ManagerList::getComponentTypeTrait(std::type_index componentTypeIndex)
	{
		auto typeTraitFound =
			std::find_if(m_componentTypeTraits.begin(), m_componentTypeTraits.end(), [componentTypeIndex](const ComponentTypeTrait& value) {
				return value.getTypeIndex() == componentTypeIndex;
			});
		return typeTraitFound != m_componentTypeTraits.end() ? std::optional(*typeTraitFound) : std::nullopt;
	}

	const std::vector<std::reference_wrapper<const ComponentTypeTrait>>& ManagerList::getComponentTypeTraits() const
	{
		return m_componentTypeTraits;
	}
}  // namespace trc