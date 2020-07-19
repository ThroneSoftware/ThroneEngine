#pragma once

#include <cassert>
#include <type_traits>

namespace tru
{
	template <typename DstType, typename SrcType>
	DstType staticDownCast(SrcType& src)
	{
		assert(dynamic_cast<std::add_pointer_t<std::remove_reference_t<DstType>>>(&src) != nullptr);
		return static_cast<DstType>(src);
	}
}  // namespace tru