#include "MockNotifier.h"

#include <Standard/Private/Resource.h>
#include <Tests/ProxyGmock.h>

#include <catch.hpp>

namespace Tests
{
	namespace TestResourcePrivate
	{
	}

	// todo, improve the Resource tests a bit

	// test destruction (the delete this)
	// tryDestroy
	// tryDestroyCtrlBlock
	// deleter

	SCENARIO("Resource construction", "Resource")
	{
		MockNotifier<int> mock;
		ProxyNotifier<int> notifier(mock);
		trs::Private::BaseResource<int>* resource = new trs::Private::CombinedResource<int, ProxyNotifier<int>>(notifier, 10);
		REQUIRE(*resource->getPtr() == 10);
		delete resource;
	}

	SCENARIO("Resource notifier", "Resource")
	{
		GIVEN("A Combined resource")
		{
			MockNotifier<int> mock;
			ProxyNotifier<int> notifier(mock);
			trs::Private::BaseResource<int>* resource = new trs::Private::CombinedResource<int, ProxyNotifier<int>>(notifier, 10);

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

		GIVEN("A separated resource")
		{
			MockNotifier<int> mock;
			ProxyNotifier<int> notifier(mock);
			int* ptr = new int(10);

			class Deleter
			{
			public:
				void operator()(int* ptr)
				{
					delete ptr;
				}
			};

			trs::Private::BaseResource<int>* resource =
				new trs::Private::SeparatedResource<int, ProxyNotifier<int>, Deleter>(notifier, Deleter(), ptr);

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
}  // namespace Tests