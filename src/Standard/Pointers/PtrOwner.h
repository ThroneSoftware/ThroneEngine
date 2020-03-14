#pragma once

#include "BasePtr.h"

namespace trs
{
	template <typename Type>
	class PtrOwner final
	{
		template <typename Type>
		friend class SharedPtr;

	public:
		using value_type = Type;
		using pointer = value_type*;
		using reference = value_type&;

	public:
		PtrOwner(Private::BaseResource<value_type>* resource) noexcept
		  : m_base(resource)
		{
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
				m_base.destroy();
				m_base = std::move(other.m_base);
			}
			return *this;
		}

		~PtrOwner() noexcept
		{
			m_base.destroy();
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