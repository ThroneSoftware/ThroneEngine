#pragma once

#include <span>
#include <vector>

namespace trg
{
	template <typename T>
	auto toVkHandle(std::span<T> span)
	{
		std::vector<typename T::VkHandleType> vec;
		vec.reserve(span.size());
		std::transform(span.begin(), span.end(), std::back_inserter(vec), [](T& value) {
			return value.getVkHandle();
		});
		return vec;
	}
}  // namespace trg