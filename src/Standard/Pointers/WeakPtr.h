#pragma once

#include "PtrOwner.h"

namespace trs
{
	template <typename Type>
	class SharedPtr;

	template <typename Type>
	class WeakPtr final
	{
		template <typename Type>
		friend class SharedPtr;

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

		WeakPtr(const SharedPtr<value_type>& sharedPtr) noexcept;

		WeakPtr(const WeakPtr& other) noexcept
		  : m_base(other.m_base)
		{
			m_base.incrementWRefCount();
		}
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

		SharedPtr<value_type> lock() noexcept;

	private:
		PointersPrivate::BasePtr<value_type> m_base;
	};
}  // namespace trs