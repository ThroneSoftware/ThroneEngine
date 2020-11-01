#pragma once

#include "BasePtr.h"

namespace trs
{
	template <typename Type>
	class PtrOwner final
	{
		template <typename Type>
		friend class SharedPtr;

		template <typename Type>
		friend class WeakPtr;

		template <typename Type>
		friend class NotifiedPtr;

	public:
		using value_type = Type;

	public:
		PtrOwner(gsl::not_null<Private::BaseResource<value_type>*> resource) noexcept
		  : m_base(resource)
		{
			m_base.incrementRefCount();
			m_base.incrementWRefCount();
		}

		PtrOwner(const PtrOwner& other) = delete;
		PtrOwner& operator=(const PtrOwner& other) = delete;

		PtrOwner(PtrOwner&& other) noexcept = default;
		PtrOwner& operator=(PtrOwner&& other) noexcept
		{
			// Only need to implement operator= since it is not a use case to construct a PtrOwner with itself.
			// Also, the base cannot be set so it does not need to be destroyed.
			if(this != &other)
			{
				destroy();
				m_base = std::move(other.m_base);
			}
			return *this;
		}

		~PtrOwner() noexcept
		{
			destroy();
		}

		value_type* getPtr() const noexcept
		{
			return m_base.getPtr();
		}

		value_type* operator->() const noexcept
		{
			return m_base.getPtr();
		}

		value_type& operator*() const noexcept
		{
			return *m_base.getPtr();
		}

		bool tryDestroy() noexcept
		{
			bool destroyed = m_base.tryDestroy();
			if(destroyed)
			{
				m_base = nullptr;
			}
			return destroyed;
		}

	private:
		void destroy() noexcept
		{
			// Keep the call out of the assert or else it will be optimized in release builds.
			bool destroyed = tryDestroy();
			assert(destroyed);
		}

		PointersPrivate::BasePtr<value_type> m_base;
	};
}  // namespace trs