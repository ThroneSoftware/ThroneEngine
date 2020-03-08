#pragma once

#include "../Private/Resource.h"

namespace trs
{
	template <typename Type>
	class EnableSharedFromThis
	{
	public:
		using value_type = Type;

	public:
		SharedPtr<value_type> makeSharedFromThis()
		{
			assert(m_resource != nullptr);
			return SharedPtr(m_resource);
		}

		void setResource(Private::BaseResource<value_type>* resource)
		{
			m_resource = resource;
		}

	private:
		Private::BaseResource<value_type>* m_resource = nullptr;
	};
}  // namespace trs