#include "BaseManager.h"

namespace trs
{
	BaseManager::BaseManager(std::type_index objectTypeIndex)
	  : m_objectTypeIndex(objectTypeIndex)
	{
	}
}  // namespace trs
