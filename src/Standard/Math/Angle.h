#pragma once

#include "Real.h"

#include <fmt/format.h>

#include <compare>
#include <numbers>

namespace trs
{
	namespace AnglePrivate
	{
		template <typename Type, Type fromUnitSize, Type toUnitSize>
		constexpr bool needAngleConversion = fromUnitSize != toUnitSize;
	}

	template <typename Type, Type fromUnitSize, Type toUnitSize>
	constexpr Type convertAngleFromTo(Type angle) noexcept requires AnglePrivate::needAngleConversion<Type, fromUnitSize, toUnitSize>
	{
		return angle * (toUnitSize / fromUnitSize);
	}

	template <typename Type, Type fromUnitSize, Type toUnitSize>
	constexpr Type convertAngleFromTo(Type angle) noexcept
	{
		return angle;
	}

	template <typename Type, Type unitSize>
	constexpr Type normalizeAngle(Type angle) noexcept requires std::is_integral_v<Type>
	{
		return angle % unitSize;
	}

	template <typename Type, Type unitSize>
	constexpr Type normalizeAngle(Type angle) noexcept requires std::is_floating_point_v<Type>
	{
		return std::fmod(angle, unitSize);
	}

	template <typename ValueType, ValueType unitSizeV>
	class Angle
	{
	public:
		using value_type = ValueType;
		constexpr static value_type unitSize = unitSizeV;
		using WrapperType = std::conditional_t<std::is_floating_point_v<value_type>, Real<value_type>, value_type>;

		constexpr Angle() noexcept = default;

		template <value_type unitSizeOther>
		constexpr Angle(Angle<value_type, unitSizeOther> other) noexcept
			requires AnglePrivate::needAngleConversion<value_type, unitSizeOther, unitSize>
		  : m_value(convertAngleFromTo<value_type, unitSizeOther, unitSize>(other.getValue()))
		{
		}

		template <value_type unitSizeOther>
		constexpr Angle& operator=(Angle<value_type, unitSizeOther> other) noexcept
			requires AnglePrivate::needAngleConversion<value_type, unitSizeOther, unitSize>
		{
			m_value = convertAngleFromTo<value_type, unitSizeOther, unitSize>(other.getValue());
			return *this;
		}

		constexpr Angle(const Angle& other) noexcept = default;
		constexpr Angle& operator=(const Angle& other) noexcept = default;

		constexpr Angle(Angle&& other) noexcept = default;
		constexpr Angle& operator=(Angle&& other) noexcept = default;

		constexpr explicit Angle(WrapperType value) noexcept
		  : m_value(value)
		{
		}

		constexpr WrapperType operator*() const noexcept
		{
			return getValue();
		}

		constexpr operator WrapperType() const noexcept
		{
			return getValue();
		}

		constexpr WrapperType getValue() const noexcept
		{
			return m_value;
		}

		constexpr void normalize() noexcept
		{
			m_value = getNormalized();
		}

		constexpr WrapperType getNormalized() noexcept
		{
			return normalizeAngle<value_type, unitSize>(m_value);
		}

		template <value_type unitSizeOther>
		constexpr std::compare_three_way_result_t<value_type, value_type>
			operator<=>(const Angle<value_type, unitSizeOther>& other) const noexcept
			requires AnglePrivate::needAngleConversion<value_type, unitSizeOther, unitSize>
		{
			return *this <=> Angle(other);
		}

		template <value_type unitSizeOther>
		constexpr bool operator==(const Angle<value_type, unitSizeOther>& other) const noexcept
			requires AnglePrivate::needAngleConversion<value_type, unitSizeOther, unitSize>
		{
			return (*this <=> other) == 0;
		}

		constexpr std::compare_three_way_result_t<value_type, value_type> operator<=>(const Angle& other) const noexcept = default;

		constexpr bool operator==(const Angle& other) const noexcept = default;

	private:
		WrapperType m_value = {};
	};

	using Radian = Angle<float, 2.0f * std::numbers::pi_v<float>>;
	using Degree = Angle<float, 360.0f>;
}  // namespace trs

namespace fmt
{
	template <typename ValueType, ValueType unitSize>
	std::string to_string(trs::Angle<ValueType, unitSize> angle)
	{
		return to_string(static_cast<trs::Angle<ValueType, unitSize>::WrapperType>(angle));
	}
}  // namespace fmt