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

	public:
		using value_type = Type;

	public:
		PtrOwner(Private::BaseResource<value_type>* resource) noexcept
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

	private:
		void destroy()
		{
			bool destroyed = m_base.destroy();
			assert(destroyed);
			m_base = nullptr;
		}

		PointersPrivate::BasePtr<value_type> m_base;
	};
}  // namespace trs