#pragma once

#include "../TypeTraits.h"
#include "NotifiedPtr.h"
#include "PtrOwner.h"
#include "SharedPtr.h"
#include "WeakPtr.h"

namespace trs
{
	template <template <typename Type> typename Pointer>
	concept IsThronePtr = is_one_of<Pointer<void>, PtrOwner<void>, NotifiedPtr<void>, SharedPtr<void>, WeakPtr<void>>();
}  // namespace trs