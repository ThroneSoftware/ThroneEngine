#pragma once

#include <ranges>

namespace tru
{
	struct MemoryRegion
	{
		template <std::ranges::range Range>
		MemoryRegion(Range&& range)
		  : data(std::ranges::data(range))
		  , byteSize(std::ranges::size(range) * sizeof(std::ranges::range_value_t<Range>))
		{
		}

		const void* data = nullptr;
		size_t byteSize = 0;
	};
}  // namespace tru