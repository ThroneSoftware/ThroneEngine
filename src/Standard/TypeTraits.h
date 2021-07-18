#pragma once

#include <type_traits>

namespace trs
{
	template <typename ReferenceType, typename... OtherTypes>
	concept IsOneOf = (std::is_same_v<ReferenceType, OtherTypes> || ...);
}  // namespace trs