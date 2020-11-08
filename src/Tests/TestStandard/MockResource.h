#pragma once

#include <Standard/Private/Resource.h>

namespace Tests
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
		MOCK_METHOD(void, destroy, (), (override, noexcept));

		MOCK_METHOD0(dtor, void());
	};

}  // namespace Tests