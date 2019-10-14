#include <catch.hpp>

#include <Standard/CompressedPair.h>

namespace Tests
{
    class EmptyClass
    {
    public:
    };

    class FinalClass final
    {
    public:
    };

    // This test is a compilation test, not an actual runnable test.
    SCENARIO("Compressed pair sizeof", "CompressedPair")
    {

        static_assert(sizeof(trs::CompressedPair<EmptyClass, int>) == sizeof(int), "Compressed pair is not compressed.");
        static_assert(sizeof(trs::CompressedPair<FinalClass, int>) == sizeof(int) * 2, "Final class is being compressed.");
        static_assert(sizeof(trs::CompressedPair<int, int>) == sizeof(int) * 2, "Fundamental types cannot be compressed.");
    }

    SCENARIO("Compressed pair construction", "CompressedPair")
    {
        GIVEN("A compressed pair")
        {
            WHEN("When constructing the pair with rvalue arguments")
            {
                THEN("arguments are properly moved")
                {

                }
            }

            WHEN("When constructing a pair with lvalue arguments")
            {
                THEN("arguments are copied")
                {

                }
            }
        }
    }
}