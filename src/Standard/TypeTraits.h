#pragma once

#include <type_traits>

namespace trs
{
	template<typename ReferenceType, typename... Types>
	constexpr bool is_one_of() noexcept {
		return (std::is_same_v<ReferenceType, Types> || ...);
	}
}