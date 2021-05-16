#define CATCH_CONFIG_RUNNER

#include <Tests/ProxyGmock.h>

#include <Vendors/Utility/Tests/GMockCatchInterceptor.hpp>
#include <Windows.h>
#include <catch.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
	testing::InitGoogleMock(&argc, argv);

	::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
	listeners.Append(new Tests::GMockCatchInterceptor());
	delete listeners.Release(listeners.default_result_printer());

	int result = Catch::Session().run(argc, argv);

	return result;
}