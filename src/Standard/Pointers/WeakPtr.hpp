#include "SharedPtr.h"
#include "WeakPtr.h"

namespace trs
{
	template <typename Type>
	inline WeakPtr<Type>::WeakPtr(const SharedPtr<value_type>& sharedPtr) noexcept
	  : m_base(sharedPtr.m_base)
	{
		m_base.incrementWRefCount();
	}

	template <typename Type>
	inline SharedPtr<Type> WeakPtr<Type>::lock() noexcept
	{
		return SharedPtr<Type>(*this);
	}
}  // namespace trs