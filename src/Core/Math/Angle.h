#pragma once

#include <numbers>

namespace trc
{
	template <typename Type, Type fromUnitSize, Type toUnitSize>
	constexpr Type convertAngleFromTo(Type angle) noexcept
	{
		return angle * (toUnitSize / fromUnitSize);
	}

	template <typename Type, Type unitSize>
	float normalizeAngle(Type angle)
	{
		return angle % unitSize;
	}

	template <typename ValueType, ValueType unitSizeV>
	class Angle
	{
	public:
		using value_type = ValueType;
		static constexpr value_type unitSize = unitSizeV;
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

		Angle(const Angle& other) = default;
		Angle& operator=(const Angle& other) = default;

		Angle(Angle&& other) = default;
		Angle& operator=(Angle&& other) = default;

		explicit constexpr Angle(value_type value) noexcept
		  : m_value(value)
		{
		}

		constexpr value_type getValue() const noexcept
		{
			return m_value;
		}

		template <value_type unitSizeOtherV>
		constexpr auto operator<=>(Angle<value_type, unitSizeOtherV> other) const noexcept requires(unitSizeOtherV != unitSize)
		{
			return m_value <=> convertAngleFromTo<value_type, unitSizeOtherV, unitSize>(other.getValue());
		}

		template <value_type unitSizeOtherV>
		constexpr auto operator==(Angle<value_type, unitSizeOtherV> other) const noexcept requires(unitSizeOtherV != unitSize)
		{
			return (*this <=> other) == 0;
		}

		constexpr auto operator<=>(Angle other) const noexcept
		{
			return m_value <=> other.m_value;
		}

		constexpr auto operator==(Angle other) const noexcept
		{
			return (*this <=> other) == 0;
		}

		void normalize()
		{
			m_value = getNormalized();
		}

		value_type getNormalized()
		{
			return normalizeAngle<value_type, unitSize>(m_value);
		}

	private:
		value_type m_value = 0.0f;
	};

	using Radian = Angle<float, 2.0f * std::numbers::pi_v<float>>;
	using Degree = Angle<float, 360.0f>;
}  // namespace trc