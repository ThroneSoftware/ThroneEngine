#pragma once

#include <compare>
#include <numbers>

namespace trc
{
	// to string

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

	template <typename Type, Type fromUnitSize, Type toUnitSize>
	constexpr Type convertAngleFromTo(Type angle) noexcept
	{
		return angle * (toUnitSize / fromUnitSize);
	}

	template <typename Type, Type fromUnitSize, Type toUnitSize>
	constexpr Type convertAngleFromTo(Type angle) noexcept requires(fromUnitSize == toUnitSize)
	{
		return angle;
	}

	template <typename Type, Type unitSize>
	constexpr Type normalizeAngle(Type angle) requires std::is_integral_v<Type>
	{
		return angle % unitSize;
	}

	template <typename Type, Type unitSize>
	constexpr Type normalizeAngle(Type angle) requires std::is_floating_point_v<Type>
	{
		return std::fmod(angle, unitSize);
	}

	template <typename ValueType, ValueType unitSizeV>
	class Angle
	{
	public:
		using value_type = ValueType;
		constexpr static value_type unitSize = unitSizeV;
		using type = Angle<value_type, unitSize>;

		constexpr Angle() noexcept = default;

		template <value_type unitSizeOtherV>
		constexpr Angle(Angle<value_type, unitSizeOtherV> other) noexcept requires(unitSizeOtherV != unitSize)
		  : m_value(convertAngleFromTo<value_type, unitSizeOtherV, unitSize>(other.getValue()))
		{
		}

		template <value_type unitSizeOtherV>
		constexpr Angle& operator=(Angle<value_type, unitSizeOtherV> other) noexcept requires(unitSizeOtherV != unitSize)
		{
			m_value = convertAngleFromTo<value_type, unitSizeOtherV, unitSize>(other.getValue());
			return *this;
		}

		constexpr Angle(const Angle& other) = default;
		constexpr Angle& operator=(const Angle& other) = default;

		constexpr Angle(Angle&& other) = default;
		constexpr Angle& operator=(Angle&& other) = default;

		constexpr explicit Angle(Real<value_type> value) noexcept
		  : m_value(value)
		{
		}

		constexpr Real<value_type> getValue() const noexcept
		{
			return m_value;
		}

		constexpr void normalize()
		{
			m_value = getNormalized();
		}

		constexpr Real<value_type> getNormalized()
		{
			return normalizeAngle<value_type, unitSize>(m_value);
		}

		template <value_type unitSizeOtherV>
		constexpr std::compare_three_way_result_t<value_type, value_type>
			operator<=>(const Angle<value_type, unitSizeOtherV>& other) const noexcept requires(unitSizeOtherV != unitSize)
		{
			return *this <=> Angle(other);
		}

		template <value_type unitSizeOtherV>
		constexpr bool operator==(const Angle<value_type, unitSizeOtherV>& other) const noexcept requires(unitSizeOtherV != unitSize)
		{
			return (*this <=> other) == 0;
		}

		constexpr std::compare_three_way_result_t<value_type, value_type> operator<=>(const Angle& other) const noexcept = default;

		constexpr bool operator==(const Angle& other) const noexcept = default;

	private:
		Real<value_type> m_value = {};
	};

	using Radian = Angle<float, 2.0f * std::numbers::pi_v<float>>;
	using Degree = Angle<float, 360.0f>;
}  // namespace trc