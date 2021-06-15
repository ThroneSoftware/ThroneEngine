#pragma once

#include <glm/glm.hpp>

#include <ranges>

namespace tru
{
	struct MemoryRegion
	{
		MemoryRegion(const void* data, size_t byteSize)
		  : data(data)
		  , byteSize(byteSize)
		{
		}

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