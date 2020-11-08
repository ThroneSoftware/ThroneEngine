#include "MockDeleter.h"
#include "MockResource.h"

#include <Standard/Private/Resource.h>
#include <Tests/ProxyGmock.h>

#include <catch.hpp>

namespace Tests
{
	SCENARIO("Resource construction", "Resource")
	{
		trs::Private::BaseResource<int>* resource = new trs::Private::CombinedResource<int>(10);
		REQUIRE(*resource->getPtr() == 10);
		delete resource;
	}

	//todo
	SCENARIO("Resource deleter", "Resource")
	{
		GIVEN("A separated resource")
		{
			MockDeleter<int> mock;
			ProxyDeleter<int> deleter(mock);
			int* ptr = new int(10);

			trs::Private::BaseResource<int>* resource = new trs::Private::SeparatedResource<int, ProxyDeleter<int>>(deleter, ptr);

			WHEN("Increasing then decreasing count")
			{
				EXPECT_CALL(mock, operatorProxy(resource->getPtr()));

				resource->incrementRefCount();
				resource->incrementRefCount();

				resource->decrementRefCount();
				resource->decrementRefCount();

				THEN("Notifier is called")
				{
					testing::Mock::VerifyAndClearExpectations(&mock);
				}
			}
			delete resource;
		}
	}

	SCENARIO("Test tryDestroy and the deleter")
	{
		GIVEN("A MockResource")
		{
			MockResource<int>* resource = new MockResource<int>();

			AND_GIVEN("1 strong ref and 1 weak ref")
			{
				resource->incrementRefCount();
				resource->incrementWRefCount();

				bool dtorCalled = false;

				EXPECT_CALL(*resource, dtor()).Times(1).WillRepeatedly([&dtorCalled]() {
					dtorCalled = true;
				});

				EXPECT_CALL(*resource, destroy()).Times(1);

				WHEN("calling tryDestroy")
				{
					bool destroyed = resource->tryDestroy();

					THEN("The resource is destroyed")
					{
						REQUIRE(destroyed == true);
						REQUIRE(dtorCalled == true);
					}
				}
			}

			AND_GIVEN("2 strong ref")
			{
				resource->incrementRefCount();
				resource->incrementRefCount();
				resource->incrementWRefCount();

				EXPECT_CALL(*resource, dtor()).Times(0);

				EXPECT_CALL(*resource, destroy()).Times(0);

				WHEN("calling tryDestroy")
				{
					bool destroyed = resource->tryDestroy();

					THEN("The resource is not destroyed")
					{
						REQUIRE(destroyed == false);

						testing::Mock::VerifyAndClearExpectations(resource);

						// cleanup
						resource->decrementRefCount();
						resource->tryDestroy();
					}
				}
			}
		}
	}

	SCENARIO("Test tryDestroyCtrlBlock")
	{
		GIVEN("A Resource")
		{
			MockResource<int>* resource = new MockResource<int>();

			bool dtorCalled = false;

			EXPECT_CALL(*resource, dtor()).Times(1).WillRepeatedly([&dtorCalled]() {
				dtorCalled = true;
			});

			AND_GIVEN("1 weak ref")
			{
				resource->incrementWRefCount();

				WHEN("calling tryDestroyCtrlBlock")
				{
					resource->tryDestroyCtrlBlock();

					THEN("Resource is destroyed")
					{
						REQUIRE(dtorCalled == true);
					}
				}
			}

			AND_GIVEN("2 weak ref")
			{
				resource->incrementWRefCount();
				resource->incrementWRefCount();

				WHEN("calling tryDestroyCtrlBlock")
				{
					resource->tryDestroyCtrlBlock();

					THEN("Resource is not destroyed")
					{
						REQUIRE(dtorCalled == false);

						// cleanup
						resource->tryDestroyCtrlBlock();
					}
				}
			}

			AND_GIVEN("1 weak ref and 1 strong ref")
			{
				resource->incrementRefCount();
				resource->incrementWRefCount();

				WHEN("calling tryDestroyCtrlBlock")
				{
					resource->tryDestroyCtrlBlock();

					THEN("Resource is not destroyed")
					{
						REQUIRE(dtorCalled == false);

						// cleanup
						resource->decrementRefCount();
						resource->incrementWRefCount();
						resource->tryDestroyCtrlBlock();
					}
				}
			}
		}
	}
}  // namespace Tests