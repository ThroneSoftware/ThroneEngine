#include <Standard/Private/Resource.h>

#include <catch.hpp>
#include <gmock.h>

namespace Tests
{
	template <typename Type>
	class MockNotifier
	{
	public:
		void operator()(Type* type)
		{
			operatorProxy(type);
		}

		MOCK_METHOD1(operatorProxy, void(int*));
	};

	// The notifier is required to be copyable but gmock mocks are not
	template <typename Type>
	class ProxyNotifier
	{
	public:
		ProxyNotifier(MockNotifier<Type>& mock)
		  : mock(mock)
		{
		}

		void operator()(Type* type)
		{
			mock(type);
		}

		MockNotifier<Type>& mock;
	};

	SCENARIO("Resource construction", "Resource")
	{
        MockNotifier<int> mock;
        ProxyNotifier<int> notifier(mock);
        trs::Private::BaseResource<int>* resource = new trs::Private::CombinedResource<int, ProxyNotifier<int>>(notifier, 10);
        REQUIRE(*resource->getPtr() == 10);
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
                EXPECT_CALL(mock, operatorProxy(testing::_));
                
                resource->increaseRefCount();
                resource->increaseRefCount();

                resource->decreaseRefCount();
                resource->decreaseRefCount();

                THEN("Notifier is called")
                {
                    testing::Mock::VerifyAndClearExpectations(&mock);
                }
            }
            delete resource;
        }
	}
}  // namespace Tests