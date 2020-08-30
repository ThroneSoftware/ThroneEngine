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
		WeakPtr(std::nullptr_t = nullptr) noexcept = default;

		WeakPtr(const PtrOwner<value_type>& ptrOwner) noexcept
		  : m_base(ptrOwner.m_base)
		{
			m_base.increaseWRefCount();
		}

		WeakPtr(const SharedPtr<value_type>& sharedPtr) noexcept
		  : m_base(sharedPtr.m_base)
		{
			m_base.increaseWRefCount();
		}

		WeakPtr(const WeakPtr& other) noexcept = default;
		WeakPtr& operator=(const WeakPtr& other) noexcept
		{
			if(this != &other)
			{
				m_base.decreaseWRefCount();
				m_base = other.m_base;
				m_base.increaseWRefCount();
			}
		}

		WeakPtr(WeakPtr&& other) noexcept = default;
		WeakPtr& operator=(WeakPtr&& other) noexcept
		{
			if(this != &other)
			{
				m_base.decreaseWRefCount();
				m_base = std::move(other.m_base);
			}
		}

		~WeakPtr() noexcept
		{
			m_base.decreaseWRefCount();
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
			// todo
		}

	private:
		PointersPrivate::BasePtr<value_type> m_base;
	};
}  // namespace trs