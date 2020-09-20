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
				m_base.tryDestroyCtrlBlock();
				m_base = other.m_base;
				m_base.incrementWRefCount();
			}
			return *this;
		}

		WeakPtr(WeakPtr&& other) noexcept = default;
		WeakPtr& operator=(WeakPtr&& other) noexcept
		{
			if(this != &other)
			{
				m_base.tryDestroyCtrlBlock();
				m_base = std::move(other.m_base);
			}
			return *this;
		}

		~WeakPtr() noexcept
		{
			m_base.tryDestroyCtrlBlock();
		}

		SharedPtr<value_type> lock() noexcept
		{
			if(m_base.incrementRefCountIfNotZero())
			{
				return SharedPtr(gsl::not_null(m_base.m_resource), SharedPtr<value_type>::TagCtorFromWeak());
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