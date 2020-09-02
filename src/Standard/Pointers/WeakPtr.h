#pragma once

#include "PtrOwner.h"
#include "SharedPtr.h"

namespace trs
{
	template <typename Type>
	class WeakPtr final
	{
	public:
		using value_type = Type;

	public:
		WeakPtr(std::nullptr_t = nullptr) noexcept
		{
		}

		WeakPtr(const PtrOwner<value_type>& ptrOwner) noexcept
		  : m_base(ptrOwner.m_base)
		{
			m_base.incrementWRefCount();
		}

		WeakPtr(const SharedPtr<value_type>& sharedPtr) noexcept
		  : m_base(sharedPtr.m_base)
		{
			m_base.incrementWRefCount();
		}

		WeakPtr(const WeakPtr& other) noexcept = default;
		WeakPtr& operator=(const WeakPtr& other) noexcept
		{
			if(this != &other)
			{
				m_base.decrementWRefCount();
				m_base = other.m_base;
				m_base.incrementWRefCount();
			}
		}

		WeakPtr(WeakPtr&& other) noexcept = default;
		WeakPtr& operator=(WeakPtr&& other) noexcept
		{
			if(this != &other)
			{
				m_base.decrementWRefCount();
				m_base = std::move(other.m_base);
			}
		}

		~WeakPtr() noexcept
		{
			m_base.decrementWRefCount();
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

		SharedPtr<value_type> lock() noexcept
		{
			if(m_base.incrementRefCountIfNotZero())
			{
				return SharedPtr(m_base.m_resource);
			}
			else
			{
				return nullptr;
			}
		}

	private:
		PointersPrivate::BasePtr<value_type> m_base;
	};
}  // namespace trs