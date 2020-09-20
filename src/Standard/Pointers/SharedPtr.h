#pragma once

#include "BasePtr.h"
#include "PtrOwner.h"

namespace trs
{
	template <typename Type>
	class SharedPtr final
	{
		template <typename Type>
		friend class WeakPtr;

		struct TagCtorFromWeak
		{
		};

	public:
		using value_type = Type;

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

		explicit SharedPtr(gsl::not_null<Private::BaseResource<value_type>*> resource, TagCtorFromWeak) noexcept
			: m_base(resource)
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
		PointersPrivate::BasePtr<value_type> m_base;
	};
}  // namespace trs