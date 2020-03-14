#include <Standard/Pointers.h>

#include <catch.hpp>
#include <gmock.h>

namespace Tests
{
	class ClassSharedFromThis : public trs::EnableSharedFromThis<ClassSharedFromThis>
	{
	};

	SCENARIO("Test the makeSharedFromThis")
	{
		GIVEN("A PtrOwner to a class that derives from EnableSharedFromThis")
		{
			auto ptrOwner = trs::makePtrOwner<ClassSharedFromThis>();
			WHEN("Making a shared from this")
			{
				auto sharedPtr = ptrOwner->makeSharedFromThis();
				THEN("The SharedPtr points to the same pointer as the PtrOwner")
				{
					REQUIRE(sharedPtr.getPtr() != nullptr);
					REQUIRE(sharedPtr.getPtr() == ptrOwner.getPtr());
				}
			}
		}
	}

}  // namespace Tests
