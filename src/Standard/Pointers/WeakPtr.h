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
		WeakPtr(std::nullptr_t ptr = nullptr) noexcept
		  : m_base(ptr)
		{
		}

		WeakPtr(const PtrOwner<value_type>& owner) noexcept
		{
			// todo
		}

		WeakPtr(const SharedPtr<value_type>& shared) noexcept
		{
			// todo
		}

		WeakPtr(const WeakPtr& other) noexcept = default;
		WeakPtr& operator=(const WeakPtr& other) noexcept
		{
			// todo
		}

		WeakPtr(WeakPtr&& other) noexcept = default;
		WeakPtr& operator=(WeakPtr&& other) noexcept
		{
			// todo
		}

		~WeakPtr() noexcept
		{
			// todo
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