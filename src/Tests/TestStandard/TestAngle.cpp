#include <Standard/Math/Angle.h>
#include <Tests/ProxyGmock.h>

#include <catch.hpp>

#include <chrono>

namespace Tests
{
	constexpr auto radValue1 = std::numbers::pi_v<float>;
	constexpr auto radValue2 = 2.0f * std::numbers::pi_v<float>;
	constexpr auto radValue3 = 0.0f;
	constexpr auto radValue4 = std::numbers::pi_v<float> / 3.3333f;

	constexpr auto degValue1 = 180.0f;
	constexpr auto degValue2 = 360.0f;
	constexpr auto degValue3 = 0.0f;
	constexpr auto degValue4 = 180.0f / 3.3333f;

	SCENARIO("Same angle conversion", "free function")
	{
		GIVEN("A radian angle")
		{
			auto rad1 = radValue1;
			auto rad2 = radValue2;
			auto rad3 = radValue3;
			auto rad4 = radValue4;

			WHEN("Converting to radian")
			{
				constexpr float radianSize = 2 * std::numbers::pi_v<float>;
				auto convertedRad1 = trs::convertAngleFromTo<float, radianSize, radianSize>(rad1);
				auto convertedRad2 = trs::convertAngleFromTo<float, radianSize, radianSize>(rad2);
				auto convertedRad3 = trs::convertAngleFromTo<float, radianSize, radianSize>(rad3);
				auto convertedRad4 = trs::convertAngleFromTo<float, radianSize, radianSize>(rad4);

				THEN("Value is not changed")
				{
					REQUIRE(convertedRad1 == rad1);
					REQUIRE(convertedRad2 == rad2);
					REQUIRE(convertedRad3 == rad3);
					REQUIRE(convertedRad4 == rad4);
				}
			}
		}

		GIVEN("A degree angle")
		{
			auto deg1 = degValue1;
			auto deg2 = degValue2;
			auto deg3 = degValue3;
			auto deg4 = degValue4;

			WHEN("Converting to degree")
			{
				auto convertedDeg1 = trs::convertAngleFromTo<float, 360.0f, 360.0f>(deg1);
				auto convertedDeg2 = trs::convertAngleFromTo<float, 360.0f, 360.0f>(deg2);
				auto convertedDeg3 = trs::convertAngleFromTo<float, 360.0f, 360.0f>(deg3);
				auto convertedDeg4 = trs::convertAngleFromTo<float, 360.0f, 360.0f>(deg4);

				THEN("Value is not changed")
				{
					REQUIRE(convertedDeg1 == deg1);
					REQUIRE(convertedDeg2 == deg2);
					REQUIRE(convertedDeg3 == deg3);
					REQUIRE(convertedDeg4 == deg4);
				}
			}
		}
	}

	SCENARIO("TestAngleClassConversion", "Angle class")
	{
		GIVEN("A radian angle")
		{
			auto rad1 = trs::Radian(radValue1);
			auto rad2 = trs::Radian(radValue2);
			auto rad3 = trs::Radian(radValue3);
			auto rad4 = trs::Radian(radValue4);

			WHEN("Converting to radian")
			{
				auto convertedRad1 = trs::Radian(rad1);
				auto convertedRad2 = trs::Radian(rad2);
				auto convertedRad3 = trs::Radian(rad3);
				auto convertedRad4 = trs::Radian(rad4);

				THEN("Value is not changed")
				{
					REQUIRE(convertedRad1 == rad1);
					REQUIRE(convertedRad2 == rad2);
					REQUIRE(convertedRad3 == rad3);
					REQUIRE(convertedRad4 == rad4);
				}
			}
			WHEN("Converting to degree")
			{
				auto convertedDeg1 = trs::Degree(rad1);
				auto convertedDeg2 = trs::Degree(rad2);
				auto convertedDeg3 = trs::Degree(rad3);
				auto convertedDeg4 = trs::Degree(rad4);

				THEN("Value is valid")
				{
					REQUIRE(convertedDeg1 == rad1);
					REQUIRE(convertedDeg2 == rad2);
					REQUIRE(convertedDeg3 == rad3);
					REQUIRE(convertedDeg4 == rad4);

					REQUIRE(convertedDeg1 == trs::Degree(degValue1));
					REQUIRE(convertedDeg2 == trs::Degree(degValue2));
					REQUIRE(convertedDeg3 == trs::Degree(degValue3));
					REQUIRE(convertedDeg4 == trs::Degree(degValue4));
				}
			}
		}

		GIVEN("A degree angle")
		{
			auto deg1 = trs::Degree(degValue1);
			auto deg2 = trs::Degree(degValue2);
			auto deg3 = trs::Degree(degValue3);
			auto deg4 = trs::Degree(degValue4);

			WHEN("Converting to degree")
			{
				auto convertedDeg1 = trs::Degree(deg1);
				auto convertedDeg2 = trs::Degree(deg2);
				auto convertedDeg3 = trs::Degree(deg3);
				auto convertedDeg4 = trs::Degree(deg4);

				THEN("Value is not changed")
				{
					REQUIRE(convertedDeg1 == deg1);
					REQUIRE(convertedDeg2 == deg2);
					REQUIRE(convertedDeg3 == deg3);
					REQUIRE(convertedDeg4 == deg4);
				}
			}
			WHEN("Converting to radian")
			{
				auto convertedRad1 = trs::Radian(deg1);
				auto convertedRad2 = trs::Radian(deg2);
				auto convertedRad3 = trs::Radian(deg3);
				auto convertedRad4 = trs::Radian(deg4);

				THEN("Value is valid")
				{
					REQUIRE(convertedRad1 == deg1);
					REQUIRE(convertedRad2 == deg2);
					REQUIRE(convertedRad3 == deg3);
					REQUIRE(convertedRad4 == deg4);

					REQUIRE(convertedRad1 == trs::Radian(radValue1));
					REQUIRE(convertedRad2 == trs::Radian(radValue2));
					REQUIRE(convertedRad3 == trs::Radian(radValue3));
					REQUIRE(convertedRad4 == trs::Radian(radValue4));
				}
			}
		}
	}

	SCENARIO("TestAngleNormalization", "Angle class")
	{
		GIVEN("A radian angle")
		{
			auto rad1 = trs::Radian(radValue1 + trs::Radian::unitSize);
			auto rad2 = trs::Radian(radValue2 + trs::Radian::unitSize);
			auto rad3 = trs::Radian(radValue3 + trs::Radian::unitSize);
			auto rad4 = trs::Radian(radValue4 + trs::Radian::unitSize);

			WHEN("Normalizing")
			{
				rad1.normalize();
				rad2.normalize();
				rad3.normalize();
				rad4.normalize();

				THEN("Value is normalized")
				{
					REQUIRE(rad1 == trs::Radian(radValue1));
					REQUIRE(rad2 == trs::Radian(0.0f));
					REQUIRE(rad3 == trs::Radian(radValue3));
					REQUIRE(rad4 == trs::Radian(radValue4));
				}
			}
		}

		GIVEN("A degree angle")
		{
			auto deg1 = trs::Degree(degValue1 + trs::Degree::unitSize);
			auto deg2 = trs::Degree(degValue2 + trs::Degree::unitSize);
			auto deg3 = trs::Degree(degValue3 + trs::Degree::unitSize);
			auto deg4 = trs::Degree(degValue4 + trs::Degree::unitSize);

			WHEN("Normalizing")
			{
				deg1.normalize();
				deg2.normalize();
				deg3.normalize();
				deg4.normalize();

				THEN("Value is normalized")
				{
					REQUIRE(deg1 == trs::Degree(degValue1));
					REQUIRE(deg2 == trs::Degree(0.0f));
					REQUIRE(deg3 == trs::Degree(degValue3));
					REQUIRE(deg4 == trs::Degree(degValue4));
				}
			}
		}
	}
}  // namespace Tests