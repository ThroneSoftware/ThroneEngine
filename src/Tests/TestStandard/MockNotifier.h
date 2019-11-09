#pragma once

#include <gmock/gmock.h>

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

	// The notifier is required to be copyable but gmock mocks are not so instead we keep a reference to the mock
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
}  // namespace Tests