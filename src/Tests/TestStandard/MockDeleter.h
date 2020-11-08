#pragma once

#include <Tests/ProxyGmock.h>

namespace Tests
{
	// The mock currently only supports Type=int
	// To add more overloads, you must add more overloads for MOCK_METHOD(operatorProxy)
	template <typename Type>
	class MockDeleter
	{
	public:
		void operator()(Type* type)
		{
			operatorProxy(type);
		}

		MOCK_METHOD1(operatorProxy, void(int*));
	};

	// The deleter is required to be movable or copyable but gmock mocks are not so instead we keep a reference to the mock
	template <typename Type>
	class ProxyDeleter
	{
	public:
		ProxyDeleter(MockDeleter<Type>& mock)
		  : mock(mock)
		{
		}

		void operator()(Type* type)
		{
			mock(type);
		}

		MockDeleter<Type>& mock;
	};
}  // namespace Tests