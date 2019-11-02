#include <pch.h>

#include <Standard/Pointers.h>

#include <catch.hpp>
#include <gmock.h>

namespace Tests
{
	// Every tests need to test leaks


	// Test make functions

	// Test construction

	// Test copies

	// Test moves

	// PtrOwner Alone

	// PtrShared Alone (this means that the PtrOwner was destroyed before PtrOwner). Do we even allow this? With delete this in the resource I think it could be possible.

	// Both ptrs in collaboration


	SCENARIO("", "Pointers")
	{
	}
}  // namespace Tests