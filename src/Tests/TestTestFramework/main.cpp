#define CATCH_CONFIG_RUNNER

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Vendors/Utility/Tests/GMockCatchInterceptor.hpp>
#include <catch.hpp>

int main(int argc, char* argv[])
{
	testing::InitGoogleMock(&argc, argv);

	::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
	listeners.Append(new Tests::GMockCatchInterceptor());
	delete listeners.Release(listeners.default_result_printer());

	int result = Catch::Session().run(argc, argv);

	return result;
}