#include <pch.h>

#include "MockNotifier.h"

#include <Standard/Pointers.h>

#include <catch.hpp>
#include <gmock.h>

namespace Tests
{
	class ClassWithMembers
	{
	public:
		ClassWithMembers(int val, std::string str)
		  : val(val)
		  , str(str)
		{
		}

		int val;
		std::string str;
	};

	SCENARIO("Test the construction of PtrOwner and SharedPtr", "Pointers")
	{
		WHEN("Constructing PtrOwner with makePtrOwner")
		{
			auto owner = trs::makePtrOwner<int>(10);
			THEN("Object contained by PtrOwner is properly constructed")
			{
				REQUIRE(owner.getPtr() != nullptr);
				REQUIRE(*owner == 10);
			}
		}

		WHEN("Constructing PtrOwner with notifier")
		{
			MockNotifier<int> mock;
			ProxyNotifier<int> notifier(mock);
			auto owner = trs::makePtrOwnerWithNotifier<int>(std::move(notifier), 10);
			THEN("Object contained by PtrOwner is properly constructed")
			{
				REQUIRE(owner.getPtr() != nullptr);
				REQUIRE(*owner == 10);
			}
		}

		WHEN("Constructing PtrOwner with complex object")
		{
			auto owner = trs::makePtrOwner<ClassWithMembers>(10, "string");
			THEN("Object contained by PtrOwner is properly constructed")
			{
				REQUIRE(owner.getPtr() != nullptr);
				REQUIRE(owner.getPtr()->val == 10);
				REQUIRE(owner.getPtr()->str == "string");
			}
		}

		WHEN("Constructing SharedPtr with PtrOwner")
		{
			auto owner = trs::makePtrOwner<int>(10);
			auto sharedPtr = trs::SharedPtr(owner);
			THEN("SharedPtr is properly constructed")
			{
				REQUIRE(sharedPtr.getPtr() != nullptr);
				REQUIRE(*sharedPtr == 10);

				REQUIRE(owner.getPtr() != nullptr);
				REQUIRE(*owner == 10);
			}
		}
	}

	SCENARIO("Test the move of PtrOwner and SharedPtr")
	{
		GIVEN("A PtrOwner")
		{
			auto owner1 = trs::makePtrOwner<int>(10);

			WHEN("Moving PtrOwner")
			{
				auto owner2 = std::move(owner1);
				THEN("PtrOwner is properly moved")
				{
					REQUIRE(owner2.getPtr() != nullptr);
					REQUIRE(*owner2 == 10);
					REQUIRE(owner1.getPtr() == nullptr);
				}
			}

			WHEN("Moving PtrOwner with assignment operator")
			{
				auto owner2 = trs::makePtrOwner<int>(5);
                owner2 = std::move(owner1);

				THEN("PtrOwner is properly moved")
				{
					REQUIRE(owner2.getPtr() != nullptr);
					REQUIRE(*owner2 == 10);
					REQUIRE(owner1.getPtr() == nullptr);
				}
			}

			WHEN("Moving PtrOwner into itself")
			{
                owner1 = std::move(owner1);
				THEN("Nothing happens")
				{
					REQUIRE(owner1.getPtr() != nullptr);
					REQUIRE(*owner1 == 10);
				}
			}
		}

		GIVEN("A SharedPtr")
		{
			auto owner = trs::makePtrOwner<int>(10);
			auto sharedPtr = trs::SharedPtr(owner);

			WHEN("Moving SharedPtr")
			{
				auto sharedPtr2 = std::move(sharedPtr);
				THEN("SharedPtr is moved")
				{
					REQUIRE(sharedPtr2.getPtr() != nullptr);
					REQUIRE(*sharedPtr2 == 10);

					REQUIRE(sharedPtr.getPtr() == nullptr);

					REQUIRE(owner.getPtr() != nullptr);
					REQUIRE(*owner == 10);
				}
			}

			WHEN("Moving SharedPtr with assignment operator")
			{
				auto owner2 = trs::makePtrOwner<int>(5);
				auto sharedPtr2 = trs::SharedPtr(owner2);

				sharedPtr2 = std::move(sharedPtr);
				THEN("SharedPtr is moved")
				{
					REQUIRE(sharedPtr.getPtr() == nullptr);

					REQUIRE(sharedPtr2.getPtr() != nullptr);
					REQUIRE(*sharedPtr2 == 10);
				}
			}

			WHEN("Moving SharedPtr into itself")
			{
				sharedPtr = std::move(sharedPtr);
				THEN("Nothing happens")
				{
					REQUIRE(sharedPtr.getPtr() != nullptr);
					REQUIRE(*sharedPtr == 10);
				}
			}
		}
	}

	SCENARIO("Test the copies of SharedPtr", "Pointers")
	{
		GIVEN("A SharedPtr")
		{
			auto owner = trs::makePtrOwner<int>(10);
			auto sharedPtr = trs::SharedPtr(owner);

			WHEN("Copying SharedPtr")
			{
				auto sharedPtr2 = sharedPtr;
				THEN("SharedPtr is copied and unchanged")
				{
					REQUIRE(sharedPtr.getPtr() == sharedPtr2.getPtr());

					REQUIRE(sharedPtr.getPtr() != nullptr);
					REQUIRE(*sharedPtr == 10);

					REQUIRE(sharedPtr2.getPtr() != nullptr);
					REQUIRE(*sharedPtr2 == 10);
				}
			}

			WHEN("Copying SharedPtr with assignment operator")
			{
				auto owner2 = trs::makePtrOwner<int>(5);
				auto sharedPtr2 = trs::SharedPtr(owner2);

				sharedPtr2 = sharedPtr;
				THEN("SharedPtr is copied and unchanged")
				{
					REQUIRE(sharedPtr.getPtr() == sharedPtr2.getPtr());

					REQUIRE(sharedPtr.getPtr() != nullptr);
					REQUIRE(*sharedPtr == 10);

					REQUIRE(sharedPtr2.getPtr() != nullptr);
					REQUIRE(*sharedPtr2 == 10);
				}
			}

			WHEN("Copying SharedPtr into itself")
			{
				sharedPtr = sharedPtr;
				THEN("Nothing happens")
				{
					REQUIRE(sharedPtr.getPtr() != nullptr);
					REQUIRE(*sharedPtr == 10);
				}
			}
		}
	}


	SCENARIO("Test that the notifier is properly called", "Pointers")
	{
		GIVEN("A PtrOwner with notifier and SharedPtrs")
		{
			MockNotifier<int> mock;
			ProxyNotifier<int> notifier(mock);
			auto owner = trs::makePtrOwnerWithNotifier<int>(notifier, 10);

			EXPECT_CALL(mock, operatorProxy(testing::_)).Times(1);

			WHEN("SharedPtrs are all destroyed")
			{
				{
					auto shared1 = trs::SharedPtr(owner);
					auto shared2 = trs::SharedPtr(owner);
					auto shared3 = trs::SharedPtr(owner);
				}

				THEN("Notifier is called")
				{
					testing::Mock::VerifyAndClearExpectations(&mock);
				}
			}
		}
	}
}  // namespace Tests