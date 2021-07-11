#pragma once

#include <numeric>

namespace trc
{
	template <typename ValueType>
	class Real
	{
	public:
		using value_type = ValueType;

		constexpr Real() noexcept = default;
		constexpr Real(value_type value)
		  : m_value(value)
		{
		}

		constexpr operator value_type() const
		{
			return m_value;
		}

		constexpr auto operator==(Real other) const noexcept
		{
			auto abs = [](auto val) {
				return val < 0 ? -val : val;
			};
			auto unitInLastPlace = static_cast<value_type>(4.0);
			auto val1 = m_value;
			auto val2 = other.m_value;
			return abs(val1 - val2) < std::numeric_limits<value_type>::epsilon() * abs(val1 + val2) * unitInLastPlace ||
				   abs(val1 - val2) < std::numeric_limits<value_type>::min();
		}

		value_type m_value = {};
	};
}  // namespace trc