#include <pch.h>

#include "MockDeleter.h"
#include "MockResource.h"

#include <Standard/Pointers.h>
#include <Tests/ProxyGmock.h>

#include <catch.hpp>

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

	template <typename Type>
	class TemplateClass
	{
	};

	SCENARIO("Test IsThronePtr concept")
	{
		REQUIRE(trs::IsThronePtr<trs::PtrOwner>);
		REQUIRE(trs::IsThronePtr<trs::NotifiedPtr>);
		REQUIRE(trs::IsThronePtr<trs::SharedPtr>);
		REQUIRE(trs::IsThronePtr<trs::WeakPtr>);

		REQUIRE(trs::IsThronePtr<TemplateClass> == false);
	}

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

		WHEN("Constructing PtrOwner with deleter")
		{
			MockDeleter<int> deleter_mock;
			ProxyDeleter<int> deleter(deleter_mock);
			auto owner = trs::makePtrOwnerWithDeleter<int>(std::move(deleter), new int(10));

			EXPECT_CALL(deleter_mock, operatorProxy(testing::_)).Times(1).WillOnce([](int* ptr) {
				delete ptr;
			});

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

	SCENARIO("Test the move of PtrOwner, SharedPtr, WeakPtr and NotifiedPtr")
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

		GIVEN("A WeakPtr")
		{
			auto owner = trs::makePtrOwner<int>(10);
			auto weakPtr = trs::WeakPtr(owner);

			WHEN("Moving WeakPtr")
			{
				auto weakPtr2 = std::move(weakPtr);
				THEN("WeakPtr is moved")
				{
					REQUIRE(weakPtr2.lock().getPtr() != nullptr);
					REQUIRE(*weakPtr2.lock() == 10);

					REQUIRE(weakPtr.lock().getPtr() == nullptr);

					REQUIRE(owner.getPtr() != nullptr);
					REQUIRE(*owner == 10);
				}
			}

			WHEN("Moving WeakPtr with assignment operator")
			{
				auto owner2 = trs::makePtrOwner<int>(5);
				auto weakPtr2 = trs::WeakPtr(owner2);

				weakPtr2 = std::move(weakPtr);
				THEN("WeakPtr is moved")
				{
					REQUIRE(weakPtr.lock().getPtr() == nullptr);

					REQUIRE(weakPtr2.lock().getPtr() != nullptr);
					REQUIRE(*weakPtr2.lock() == 10);
				}
			}

			WHEN("Moving WeakPtr into itself")
			{
				weakPtr = std::move(weakPtr);
				THEN("Nothing happens")
				{
					REQUIRE(weakPtr.lock().getPtr() != nullptr);
					REQUIRE(*weakPtr.lock() == 10);
				}
			}
		}

		GIVEN("A NotifiedPtr")
		{
			auto owner = trs::makePtrOwner<int>(10);
			auto notifiedPtr = trs::NotifiedPtr(owner);

			WHEN("Moving NotifiedPtr")
			{
				auto notifiedPtr2 = std::move(notifiedPtr);
				THEN("NotifiedPtr is moved")
				{
					REQUIRE(notifiedPtr2.getPtr() != nullptr);
					REQUIRE(*notifiedPtr2 == 10);

					REQUIRE(notifiedPtr.getPtr() == nullptr);

					REQUIRE(owner.getPtr() != nullptr);
					REQUIRE(*owner == 10);
				}
			}

			WHEN("Moving NotifiedPtr with assignment operator")
			{
				auto owner2 = trs::makePtrOwner<int>(5);
				auto notifiedPtr2 = trs::NotifiedPtr(owner2);

				notifiedPtr2 = std::move(notifiedPtr);
				THEN("NotifiedPtr is moved")
				{
					REQUIRE(notifiedPtr.getPtr() == nullptr);

					REQUIRE(notifiedPtr2.getPtr() != nullptr);
					REQUIRE(*notifiedPtr2 == 10);
				}
			}

			WHEN("Moving NotifiedPtr into itself")
			{
				notifiedPtr = std::move(notifiedPtr);
				THEN("Nothing happens")
				{
					REQUIRE(notifiedPtr.getPtr() != nullptr);
					REQUIRE(*notifiedPtr == 10);
				}
			}
		}
	}

	SCENARIO("Test the copies of SharedPtr, WeakPtr and NotifiedPtr", "Pointers")
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

		GIVEN("A WeakPtr")
		{
			auto owner = trs::makePtrOwner<int>(10);
			auto weakPtr = trs::WeakPtr(owner);

			WHEN("Copying WeakPtr")
			{
				auto weakPtr2 = weakPtr;
				THEN("WeakPtr is copied and unchanged")
				{
					REQUIRE(weakPtr.lock().getPtr() == weakPtr2.lock().getPtr());

					REQUIRE(weakPtr.lock().getPtr() != nullptr);
					REQUIRE(*weakPtr.lock() == 10);

					REQUIRE(weakPtr2.lock().getPtr() != nullptr);
					REQUIRE(*weakPtr2.lock() == 10);
				}
			}

			WHEN("Copying WeakPtr with assignment operator")
			{
				auto owner2 = trs::makePtrOwner<int>(5);
				auto weakPtr2 = trs::WeakPtr(owner2);

				weakPtr2 = weakPtr;
				THEN("WeakPtr is copied and unchanged")
				{
					REQUIRE(weakPtr.lock().getPtr() == weakPtr2.lock().getPtr());

					REQUIRE(weakPtr.lock().getPtr() != nullptr);
					REQUIRE(*weakPtr.lock() == 10);

					REQUIRE(weakPtr2.lock().getPtr() != nullptr);
					REQUIRE(*weakPtr2.lock() == 10);
				}
			}

			WHEN("Copying WeakPtr into itself")
			{
				weakPtr = weakPtr;
				THEN("Nothing happens")
				{
					REQUIRE(weakPtr.lock().getPtr() != nullptr);
					REQUIRE(*weakPtr.lock() == 10);
				}
			}
		}

		GIVEN("A NotifiedPtr")
		{
			auto owner = trs::makePtrOwner<int>(10);
			auto notifiedPtr = trs::NotifiedPtr(owner);

			WHEN("Copying NotifiedPtr")
			{
				auto notifiedPtr2 = notifiedPtr;
				THEN("NotifiedPtr is copied and unchanged")
				{
					REQUIRE(notifiedPtr.getPtr() == notifiedPtr2.getPtr());

					REQUIRE(notifiedPtr.getPtr() != nullptr);
					REQUIRE(*notifiedPtr == 10);

					REQUIRE(notifiedPtr2.getPtr() != nullptr);
					REQUIRE(*notifiedPtr2 == 10);
				}
			}

			WHEN("Copying NotifiedPtr with assignment operator")
			{
				auto owner2 = trs::makePtrOwner<int>(5);
				auto notifiedPtr2 = trs::NotifiedPtr(owner2);

				notifiedPtr2 = notifiedPtr;
				THEN("NotifiedPtr is copied and unchanged")
				{
					REQUIRE(notifiedPtr.getPtr() == notifiedPtr2.getPtr());

					REQUIRE(notifiedPtr.getPtr() != nullptr);
					REQUIRE(*notifiedPtr == 10);

					REQUIRE(notifiedPtr2.getPtr() != nullptr);
					REQUIRE(*notifiedPtr2 == 10);
				}
			}

			WHEN("Copying NotifiedPtr into itself")
			{
				notifiedPtr = notifiedPtr;
				THEN("Nothing happens")
				{
					REQUIRE(notifiedPtr.getPtr() != nullptr);
					REQUIRE(*notifiedPtr == 10);
				}
			}
		}
	}

	SCENARIO("Test WeakPtr::lock")
	{
		GIVEN("A WeakPtr that points to a ptr that is still valid")
		{
			auto owner = trs::makePtrOwner<int>(10);
			auto weakPtr = trs::WeakPtr(owner);

			WHEN("Locking the WeakPtr")
			{
				auto sharedPtr = weakPtr.lock();

				THEN("The SharedPtr is valid")
				{
					REQUIRE(sharedPtr.getPtr() == owner.getPtr());
					REQUIRE(*sharedPtr == *owner);
				}
			}
		}

		GIVEN("A WeakPtr that points to a ptr that is no longer valid")
		{
			auto owner = trs::makePtrOwner<int>(10);
			auto weakPtr = trs::WeakPtr(owner);

			owner.tryDestroy();

			WHEN("Locking the WeakPtr")
			{
				auto sharedPtr = weakPtr.lock();

				THEN("The sharedPtr is null")
				{
					REQUIRE(sharedPtr.getPtr() == nullptr);
				}
			}
		}
	}

	SCENARIO("Test resource destruction")
	{
		GIVEN("A Resource")
		{
			auto uptr = std::make_unique<int>(10);
			auto ptr = uptr.get();

			MockResource<int>* resource = new MockResource<int>();
			ON_CALL(*resource, getPtr()).WillByDefault(testing::Return(ptr));

			bool dtorCalled = false;

			EXPECT_CALL(*resource, dtor()).Times(testing::AtMost(1)).WillRepeatedly([&dtorCalled]() {
				dtorCalled = true;
			});

			AND_GIVEN("A PtrOwner that uses the resource")
			{
				auto owner = trs::PtrOwner<int>(resource);

				WHEN("calling tryDestroy")
				{
					EXPECT_CALL(*resource, destroy()).Times(1);
					owner.tryDestroy();

					THEN("Resource's dtor is called")
					{
						REQUIRE(dtorCalled == true);
					}
				}

				AND_GIVEN("A SharedPtr that uses the owner")
				{
					auto shared = trs::SharedPtr(owner);

					WHEN("calling tryDestroy")
					{
						EXPECT_CALL(*resource, destroy()).Times(0);
						owner.tryDestroy();

						THEN("Resource's dtor is not called")
						{
							REQUIRE(dtorCalled == false);

							// cleanup
							testing::Mock::VerifyAndClearExpectations(resource);
						}
					}
				}

				AND_GIVEN("A WeakPtr that uses the owner")
				{
					auto weak = trs::WeakPtr(owner);

					WHEN("calling tryDestroy")
					{
						EXPECT_CALL(*resource, destroy()).Times(1);
						owner.tryDestroy();

						THEN("Resource's dtor is not called")
						{
							REQUIRE(dtorCalled == false);
						}
					}
				}
			}
		}
	}

	SCENARIO("Test NotifiedPtr's notification")
	{
		GIVEN("A PtrOwner and a NotifiedPtr")
		{
			auto ptrOwner = trs::makePtrOwner<int>(10);
			auto notifiedPtr = trs::NotifiedPtr(ptrOwner);

			WHEN("Destroying the PtrOwner")
			{
				auto destroyed = ptrOwner.tryDestroy();
				REQUIRE(destroyed);

				THEN("The NotifiedPtr is set to nullptr")
				{
					REQUIRE(notifiedPtr.getPtr() == nullptr);
				}
			}
		}
	}
}  // namespace Tests