#pragma once

#include <functional>
#include <span>
#include <vector>

namespace trg
{
	template <typename T, std::ranges::range Range>
	auto rangeToVkHandle(Range&& range)
	{
		std::vector<typename T::VkHandleType> vec;
		vec.reserve(std::ranges::size(range));
		std::transform(std::ranges::begin(range), std::ranges::end(range), std::back_inserter(vec), [](T& value) {
			return value.getVkHandle();
		});
		return vec;
	}

	template <typename T>
	auto toVkHandle(std::span<T> span)
	{
		return rangeToVkHandle<T>(span);
	}

	template <typename T>
	auto toVkHandle(std::vector<std::reference_wrapper<T>> references)
	{
		return rangeToVkHandle<T>(references);
	}
}  // namespace trg