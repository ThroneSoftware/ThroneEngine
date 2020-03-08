#pragma once

#include "../Private/Resource.h"
#include "SharedPtr.h"

namespace trs
{
	namespace PointersPrivate
	{
		template <typename Type>
		class EnableSharedFromThisSetResourceAccessor;
	}

	template <typename Type>
	class EnableSharedFromThis
	{
	public:
		using value_type = Type;

		template <typename Type>
		friend class PointersPrivate::EnableSharedFromThisSetResourceAccessor;

	public:
		SharedPtr<value_type> makeSharedFromThis()
		{
			assert(m_resource != nullptr);
			return SharedPtr(m_resource);
		}

	private:
		void setResource(Private::BaseResource<value_type>* resource)
		{
			m_resource = resource;
		}

	private:
		Private::BaseResource<value_type>* m_resource = nullptr;
	};

	namespace PointersPrivate
	{
		template <typename Type>
		class EnableSharedFromThisSetResourceAccessor
		{
		public:
			using value_type = Type;

		public:
			void setResource(EnableSharedFromThis<value_type>* ptr, Private::BaseResource<value_type>* resource)
			{
				ptr->setResource(resource);
			}
		};
	}  // namespace PointersPrivate
}  // namespace trs