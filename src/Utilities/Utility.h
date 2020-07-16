#pragma once

#include <cassert>

namespace tru
{
	template <typename DstType, typename SrcType>
	DstType staticDownCast(SrcType& src)
	{
		assert(dynamic_cast<DstType>(src) != nullptr);
		return static_cast<DstType>(src);
	}
}  // namespace tru