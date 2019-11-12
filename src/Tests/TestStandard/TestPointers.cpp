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
			auto ptr = trs::makePtrOwner<int>(10);
			THEN("Object contained by PtrOwner is properly constructed")
			{
                REQUIRE(ptr.getPtr() != nullptr);
				REQUIRE(*ptr == 10);
			}
		}

		WHEN("Constructing PtrOwner with notifier")
		{
			MockNotifier<int> mock;
			ProxyNotifier<int> notifier(mock);
			auto ptr = trs::makePtrOwnerWithNotifier<int, ProxyNotifier<int>>(std::move(notifier), 10);
			THEN("Object contained by PtrOwner is properly constructed")
			{
                REQUIRE(ptr.getPtr() != nullptr);
				REQUIRE(*ptr == 10);
			}
		}

		WHEN("Constructing PtrOwner with complex object")
		{
			auto ptr = trs::makePtrOwner<ClassWithMembers>(10, "string");
			THEN("Object contained by PtrOwner is properly constructed")
			{
                REQUIRE(ptr.getPtr() != nullptr);
				REQUIRE(ptr.getPtr()->val == 10);
				REQUIRE(ptr.getPtr()->str == "string");
			}
		}

        WHEN("Constructing SharedPtr with PtrOwner")
        {
            auto ptr = trs::makePtrOwner<int>(10);
            auto sharedPtr = trs::SharedPtr(ptr);
            THEN("SharedPtr is properly constructed")
            {
                REQUIRE(sharedPtr.getPtr() != nullptr);
                REQUIRE(*sharedPtr == 10);

                REQUIRE(ptr.getPtr() != nullptr);
                REQUIRE(*ptr == 10);
            } 
        }
	}

    SCENARIO("Test the move of PtrOwner and SharedPtr")
    {
        GIVEN("A PtrOwner")
        {
            auto ptr = trs::makePtrOwner<int>(10);

            WHEN("Moving PtrOwner")
            {
                auto ptr2 = std::move(ptr);
                THEN("PtrOwner is properly moved")
                {
                    REQUIRE(ptr2.getPtr() != nullptr);
                    REQUIRE(*ptr2 == 10);
                    REQUIRE(ptr.getPtr() == nullptr);
                }
            }

            WHEN("Moving PtrOwner with assignment operator")
            {
                auto ptr2 = trs::makePtrOwner<int>(5);
                ptr2 = std::move(ptr);

                THEN("PtrOwner is properly moved")
                {
                    REQUIRE(ptr2.getPtr() != nullptr);
                    REQUIRE(*ptr2 == 10);
                    REQUIRE(ptr.getPtr() == nullptr);
                }
            }

            WHEN("Moving PtrOwner into itself")
            {
                ptr = std::move(ptr);
                THEN("Nothing happens")
                {
                    REQUIRE(ptr.getPtr() != nullptr);
                    REQUIRE(*ptr == 10);
                }
            }
        }

        GIVEN("A SharedPtr")
        {
            auto ptr = trs::makePtrOwner<int>(10);
            auto sharedPtr = trs::SharedPtr(ptr);

            WHEN("Moving SharedPtr")
            {
                auto sharedPtr2 = std::move(sharedPtr);
                THEN("SharedPtr is moved")
                {
                    REQUIRE(sharedPtr2.getPtr() != nullptr);
                    REQUIRE(*sharedPtr2 == 10);

                    REQUIRE(sharedPtr.getPtr() == nullptr);

                    REQUIRE(ptr.getPtr() != nullptr);
                    REQUIRE(*ptr == 10);
                }
            }

            WHEN("Moving SharedPtr with assignment operator")
            {
                auto ptr2 = trs::makePtrOwner<int>(5);
                auto sharedPtr2 = trs::SharedPtr(ptr2);
                
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
            auto ptr = trs::makePtrOwner<int>(10);
            auto sharedPtr = trs::SharedPtr(ptr);

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
                auto ptr2 = trs::makePtrOwner<int>(5);
                auto sharedPtr2 = trs::SharedPtr(ptr);

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
            auto ptrOwner = trs::makePtrOwnerWithNotifier<int>(notifier, 10);

            EXPECT_CALL(mock, operatorProxy(testing::_)).Times(1);

            WHEN("SharedPtrs are all destroyed")
            {
                {
                    auto shared1 = trs::SharedPtr(ptrOwner);
                    auto shared2 = trs::SharedPtr(ptrOwner);
                    auto shared3 = trs::SharedPtr(ptrOwner);
                }

                THEN("Notifier is called")
                {
                    testing::Mock::VerifyAndClearExpectations(&mock);
                }
            }
        }
    }
}  // namespace Tests