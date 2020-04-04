#pragma once

#include "PtrOwner.h"
#include "SharedPtr.h"

namespace trs
{
	template <typename Type>
	inline bool operator==(const trs::SharedPtr<Type>& ptr, std::nullptr_t)
	{
		return ptr.getPtr() == nullptr;
	}

	template <typename Type>
	inline bool operator==(const trs::SharedPtr<Type>& lhs, const trs::SharedPtr<Type>& rhs)
	{
		return lhs.getPtr() == rhs.getPtr();
	}

	template <typename Type>
	inline bool operator==(Type* lhs, const trs::SharedPtr<Type>& rhs)
	{
		return lhs == rhs.getPtr();
	}

	template <typename Type>
	inline bool operator==(const trs::SharedPtr<Type>& lhs, Type* rhs)
	{
		return lhs.getPtr() == rhs;
	}

	template <typename Type>
	inline bool operator==(const trs::SharedPtr<Type>& lhs, const trs::PtrOwner<Type>& rhs)
	{
		return lhs.getPtr() == rhs.getPtr();
	}

	template <typename Type>
	inline bool operator==(const trs::PtrOwner<Type>& lhs, const trs::SharedPtr<Type>& rhs)
	{
		return lhs.getPtr() == rhs.getPtr();
	}
}  // namespace trs