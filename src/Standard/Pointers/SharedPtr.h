#pragma once

#include "BasePtr.h"
#include "PtrOwner.h"
#include "WeakPtr.h"

namespace trs
{
	template <typename Type>
	class SharedPtr final
	{
	public:
		using value_type = Type;

		template <typename Type>
		friend class WeakPtr;

	public:
		SharedPtr(std::nullptr_t = nullptr) noexcept
		{
		}

		explicit SharedPtr(const PtrOwner<value_type>& ptrOwner) noexcept
		  : m_base(ptrOwner.m_base)
		{
			m_base.incrementRefCount();
		}

		explicit SharedPtr(gsl::not_null<Private::BaseResource<value_type>*> resource) noexcept
		  : m_base(resource)
		{
			m_base.incrementRefCount();
		}

		explicit SharedPtr(WeakPtr<value_type>& weak)
		  : m_base(constructFromWeak(weak))
		{
		}

		SharedPtr(const SharedPtr& other) noexcept
		  : m_base(other.m_base)
		{
			m_base.incrementRefCount();
		}

		SharedPtr& operator=(const SharedPtr& other) noexcept
		{
			if(this != &other)
			{
				m_base.decrementRefCount();
				m_base = other.m_base;
				m_base.incrementRefCount();
			}
			return *this;
		}

		SharedPtr(SharedPtr&& other) noexcept = default;
		SharedPtr& operator=(SharedPtr&& other) noexcept
		{
			// Only need to implement operator= since it is not a use case to construct a SharedPtr with itself.
			// Also, the base cannot be set so its ref count does not need to be decreased.
			if(this != &other)
			{
				m_base.decrementRefCount();
				m_base = std::move(other.m_base);
			}
			return *this;
		}

		~SharedPtr() noexcept
		{
			reset();
		}

		void reset() noexcept
		{
			m_base.decrementRefCount();
			m_base = nullptr;
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
		PointersPrivate::BasePtr<value_type> constructFromWeak(WeakPtr<value_type>& weak)
		{
			if(weak.m_base.incrementRefCountIfNotZero())
			{
				return weak.m_base;
			}
			else
			{
				return nullptr;
			}
		}

		PointersPrivate::BasePtr<value_type> m_base;
	};
}  // namespace trs