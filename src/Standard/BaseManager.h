#pragma once

#include <typeindex>

namespace trs
{
	class BaseManager
	{
	public:
		BaseManager(std::type_index objectTypeIndex);

		virtual ~BaseManager() = default;

		std::type_index getObjectTypeIndex() const
		{
			return m_objectTypeIndex;
		}

	private:
		const std::type_index m_objectTypeIndex;
	};
}  // namespace trs