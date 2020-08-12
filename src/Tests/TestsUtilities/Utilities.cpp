#pragma once

#include <catch.hpp>

#include <atomic>

namespace Tests
{
	class UniqueCounter
	{
	public:
		int operator++()
		{
			int c = ++counter;
			REQUIRE(c == 1);
			return c;
		}

		int operator--()
		{
			int c = --counter;
			REQUIRE(c == 0);
			return c;
		}

	private:
		std::atomic_int counter = 0;
	};
}  // namespace Tests