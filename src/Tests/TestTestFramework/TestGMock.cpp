#include <gmock/gmock.h>

#include <catch.hpp>

namespace Tests
{
	class TestBaseClass
	{
	public:
		virtual int func(bool) = 0;
	};

	class MockBaseClass : public TestBaseClass
	{
	public:
		MOCK_METHOD1(func, int(bool));
	};

	static_assert(!std::is_abstract_v<MockBaseClass>, "MockBaseClass should not be abstact");


	TEST_CASE("Test gmock", "gmock")
	{
		MockBaseClass mockBaseClass;

		SECTION("Test expect call")
		{
			EXPECT_CALL(mockBaseClass, func);
			mockBaseClass.func(true);
		}

		SECTION("Test argument")
		{
			EXPECT_CALL(mockBaseClass, func(true));
			mockBaseClass.func(true);
		}
	}
}  // namespace Tests