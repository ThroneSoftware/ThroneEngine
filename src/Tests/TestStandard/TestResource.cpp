#include "MockNotifier.h"

#include <Standard/Private/Resource.h>
#include <Tests/ProxyGmock.h>

#include <catch.hpp>

namespace Tests
{
	namespace TestResourcePrivate
	{
		template <typename T>
		class MockResource : public trs::Private::BaseResource<T>
		{
		public:
			~MockResource() override
			{
				dtor();
			}

			MOCK_METHOD(T*, getPtr, (), (override, noexcept));
			MOCK_METHOD(void, notify, (), (override, noexcept));
			MOCK_METHOD(void, destroy, (), (override, noexcept));

			MOCK_METHOD0(dtor, void());
		};

	}  // namespace TestResourcePrivate

	// todo, improve the Resource tests a bit

	// tryDestroy
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

	SCENARIO("Test tryDestroyCtrlBlock")
	{
		GIVEN("A Resource")
		{
			TestResourcePrivate::MockResource<int>* resource = new TestResourcePrivate::MockResource<int>();

			bool dtorCalled = false;

			EXPECT_CALL(*resource, dtor()).Times(testing::Exactly(1)).WillRepeatedly([&dtorCalled]() {
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
						delete resource;
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
						delete resource;
					}
				}
			}
		}
	}
}  // namespace Tests