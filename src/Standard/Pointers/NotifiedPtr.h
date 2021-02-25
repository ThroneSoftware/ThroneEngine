#pragma once

#include "BasePtr.h"
#include "PtrOwner.h"

namespace trs
{
	template <typename Type>
	class NotifiedPtr final
	{
	public:
		using value_type = Type;

	public:
		NotifiedPtr(std::nullptr_t = nullptr) noexcept
		  : m_base(nullptr)
		{
		}

		NotifiedPtr(const PtrOwner<value_type>& ptrOwner)
		  : m_base(ptrOwner.m_base)
		{
			m_base.incrementWRefCount();

			m_base.addNotifiedPtr();
		}

		NotifiedPtr(const NotifiedPtr& other)
		  : m_base(other.m_base)
		{
			m_base.incrementWRefCount();

			m_base.addNotifiedPtr();
		}
		NotifiedPtr& operator=(const NotifiedPtr& other)
		{
			if(this != &other)
			{
				m_base.removeNotifiedPtr();
				m_base.tryDestroyCtrlBlock();

				m_base = other.m_base;
				m_base.incrementWRefCount();
				m_base.addNotifiedPtr();
			}
			return *this;
		}

		NotifiedPtr(NotifiedPtr&& other)
		{
			other.m_base.removeNotifiedPtr();

			m_base = std::move(other.m_base);
			m_base.incrementWRefCount();
			m_base.addNotifiedPtr();
		}
		NotifiedPtr& operator=(NotifiedPtr&& other)
		{
			if(this != &other)
			{
				other.m_base.removeNotifiedPtr();

				m_base.removeNotifiedPtr();
				m_base.tryDestroyCtrlBlock();

				m_base = std::move(other.m_base);
				m_base.incrementWRefCount();
				m_base.addNotifiedPtr();
			}
			return *this;
		}

		~NotifiedPtr()
		{
			reset();
		}

		void reset() noexcept
		{
			m_base.removeNotifiedPtr();
			m_base.tryDestroyCtrlBlock();
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