#include "ManagerList.h"

namespace trc
{
	const std::vector<std::reference_wrapper<const ComponentTypeTrait>>& ManagerList::getComponentTypeTraits()
	{
		return m_componentTypeTraits;
	}
}  // namespace trc