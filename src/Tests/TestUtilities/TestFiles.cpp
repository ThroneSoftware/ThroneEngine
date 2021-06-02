#include <Utilities/Files.h>

#include <catch.hpp>

namespace Tests
{
	SCENARIO("Test validateFile")
	{
		std::filesystem::current_path(std::filesystem::path(__FILE__).parent_path());

		auto extension = ".txt";

		// No extension
		REQUIRE_THROWS(tru::validateFile("./TestData/invalid", extension));
		// Not a file
		REQUIRE_THROWS(tru::validateFile("./TestData", extension));
		// Wrong extension
		REQUIRE_THROWS(tru::validateFile("./TestData/File.t", extension));
		// File does not exists
		REQUIRE_THROWS(tru::validateFile("./TestData/File2.txt", extension));

		REQUIRE_NOTHROW(tru::validateFile("./TestData/File.txt", extension));
	}
}  // namespace Tests