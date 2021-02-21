#pragma once

#include <type_traits>

namespace trs
{
	template<typename ReferenceType, typename... Types>
	constexpr bool isOneOf() noexcept {
		return (std::is_same_v<ReferenceType, Types> || ...);
	}
}