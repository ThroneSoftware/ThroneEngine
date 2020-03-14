#pragma once

#include "../Private/Resource.h"
#include "SharedPtr.h"

namespace trs
{
	namespace PointersPrivate
	{
		template <typename Type>
		class EnableSharedFromThisPrivate
		{
		public:
			using value_type = Type;

		public:
			EnableSharedFromThisPrivate() = default;

			EnableSharedFromThisPrivate(const EnableSharedFromThisPrivate& other) = delete;
			EnableSharedFromThisPrivate& operator=(const EnableSharedFromThisPrivate& other) = delete;

			EnableSharedFromThisPrivate(EnableSharedFromThisPrivate&& other)
			{
				assert(this != &other);
				m_resource = other.m_resource;
				other.m_resource = nullptr;
			}

			EnableSharedFromThisPrivate& operator=(EnableSharedFromThisPrivate&& other)
			{
				if(this != &other)
				{
					m_resource = other.m_resource;
					other.m_resource = nullptr;
				}
			}

			virtual ~EnableSharedFromThisPrivate() = default;

			void setResource(Private::BaseResource<value_type>* resource)
			{
				assert(m_resource == nullptr);
				m_resource = resource;
			}

		protected:
			Private::BaseResource<value_type>* m_resource = nullptr;
		};
	}  // namespace PointersPrivate

	template <typename Type>
	class EnableSharedFromThis : private PointersPrivate::EnableSharedFromThisPrivate<Type>
	{
	public:
		using value_type = Type;

	public:
		SharedPtr<value_type> makeSharedFromThis()
		{
			assert(PointersPrivate::EnableSharedFromThisPrivate<Type>::m_resource != nullptr);
			return SharedPtr(PointersPrivate::EnableSharedFromThisPrivate<Type>::m_resource);
		}
	};
}  // namespace trs