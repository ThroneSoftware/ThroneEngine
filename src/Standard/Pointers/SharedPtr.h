#pragma once

#include "BasePtr.h"
#include "PtrOwner.h"

namespace trs
{
	template <typename Type>
	class SharedPtr final
	{
	public:
		using value_type = Type;
		using pointer = value_type*;
		using reference = value_type&;

	public:
		SharedPtr() noexcept
		  : m_base(nullptr)
		{
		}

		SharedPtr(std::nullptr_t) noexcept
		  : SharedPtr<value_type>()
		{
		}

		explicit SharedPtr(const PtrOwner<value_type>& ptrOwner) noexcept
		  : m_base(ptrOwner.m_base)
		{
			m_base.increaseRefCount();
		}

		explicit SharedPtr(Private::BaseResource<value_type>* resource) noexcept
		  : m_base(resource)
		{
			m_base.increaseRefCount();
		}

		SharedPtr(const SharedPtr& other) noexcept
		  : m_base(other.m_base)
		{
			m_base.increaseRefCount();
		}

		SharedPtr& operator=(const SharedPtr& other) noexcept
		{
			if(this != &other)
			{
				m_base.decreaseRefCount();
				m_base = other.m_base;
				m_base.increaseRefCount();
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
				m_base.decreaseRefCount();
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
			m_base.decreaseRefCount();
			m_base = nullptr;
		}

		pointer getPtr() const noexcept
		{
			return m_base.getPtr();
		}

		pointer operator->() const noexcept
		{
			return m_base.getPtr();
		}

		reference operator*() const noexcept
		{
			return *m_base.getPtr();
		}

	private:
		PointersPrivate::BasePtr<value_type> m_base;
	};
}  // namespace trs