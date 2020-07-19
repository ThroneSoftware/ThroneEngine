#include "ManagerList.h"

namespace trc
{
	const std::vector<std::reference_wrapper<const ComponentTypeTrait>>& ManagerList::getComponentTypeTraits() const
	{
		return m_componentTypeTraits;
	}
}  // namespace trc