#pragma once

#include "../../Googletest/googlemock/include/gmock/gmock.h"
#include "catch.hpp"

namespace Tests
{
	class GMockCatchInterceptor : public ::testing::EmptyTestEventListener
	{
	public:
		GMockCatchInterceptor() = default;
		~GMockCatchInterceptor() override = default;

		void formatMessage(std::string& message, const testing::TestPartResult& gmockResult)
		{
			if(gmockResult.file_name() != nullptr)
			{
				message += "File: " + std::string(gmockResult.file_name());
				message += "\n";
			}
			if(gmockResult.line_number() != -1)
			{
				message += "Line: " + std::string(std::to_string(gmockResult.line_number()));
				message += "\n";
			}
			if(gmockResult.summary() != nullptr)
			{
				message += std::string(gmockResult.summary());
				message += "\n";
			}
		}

		// Called after a failed assertion or a SUCCEED() invocation.
		void OnTestPartResult(const testing::TestPartResult& gmockResult)
		{
			if(gmockResult.failed() && gmockResult.type() == testing::TestPartResult::kFatalFailure)
			{
				std::string message = std::string("GMock Failure:\n");
				formatMessage(message, gmockResult);
				FAIL_CHECK(message.c_str());
			}
			else if(gmockResult.type() == testing::TestPartResult::kNonFatalFailure)
			{
				std::string message = std::string("GMock Warning:\n");
				formatMessage(message, gmockResult);
				FAIL_CHECK(message.c_str());
			}
		}

		void OnTestProgramStart(const testing::UnitTest& unit_test)
		{
			//printf("OnTestProgramStart\n");
		}
		void OnTestIterationStart(const testing::UnitTest& unit_test, int iteration)
		{
			//printf("OnTestIterationStart\n");
		}
		void OnEnvironmentsSetUpStart(const testing::UnitTest& unit_test)
		{
			//printf("OnEnvironmentsSetUpStart\n");
		}
		void OnEnvironmentsSetUpEnd(const testing::UnitTest& unit_test)
		{
			//printf("OnEnvironmentsSetUpEnd\n");
		}
		void OnTestCaseStart(const testing::TestCase& test_case)
		{
			//printf("OnTestCaseStart\n");
		}
		void OnTestStart(const testing::TestInfo& test_info)
		{
			//printf("OnTestStart\n");
		}
		void OnTestEnd(const testing::TestInfo& test_info)
		{
			//printf("OnTestEnd\n");
		}
		void OnTestCaseEnd(const testing::TestCase& test_case)
		{
			//printf("OnTestCaseEnd\n");
		}
		void OnEnvironmentsTearDownStart(const testing::UnitTest& unit_test)
		{
			//printf("OnEnvironmentsTearDownStart\n");
		}
		void OnEnvironmentsTearDownEnd(const testing::UnitTest& unit_test)
		{
			//printf("OnEnvironmentsTearDownEnd\n");
		}
		void OnTestIterationEnd(const testing::UnitTest& unit_test, int iteration)
		{
			//printf("OnTestIterationEnd\n");
		}
		void OnTestProgramEnd(const testing::UnitTest& unit_test)
		{
			//printf("OnTestProgramEnd\n");
		}
	};
}  // namespace Tests