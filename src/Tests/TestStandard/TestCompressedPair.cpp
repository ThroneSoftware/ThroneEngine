#include <pch.h>

#include <Standard/CompressedPair.h>

#include <catch.hpp>
#include <gmock.h>

namespace Tests
{
	//This test is a compilation test, not a runtime test.
	TEST_CASE("Compressed pair sizeof", "CompressedPair")
	{
		class EmptyClass
		{
		public:
		};

		class FinalClass final
		{
		public:
		};

		static_assert(sizeof(trs::CompressedPair<EmptyClass, int>) == sizeof(int), "Compressed pair is not compressed.");
		static_assert(sizeof(trs::CompressedPair<int, EmptyClass>) == sizeof(int), "Compressed pair is not compressed.");
		static_assert(sizeof(trs::CompressedPair<FinalClass, int>) == sizeof(int) * 2, "Final class is being compressed.");
		static_assert(sizeof(trs::CompressedPair<int, FinalClass>) == sizeof(int) * 2, "Final class is being compressed.");
		static_assert(sizeof(trs::CompressedPair<int, int>) == sizeof(int) * 2, "Fundamental types cannot be compressed.");
	}

	template <typename Type1, typename Type2, typename Obj>
	void constructWithMove(Obj&& move)
	{
		trs::CompressedPair<Type1, Type2> pair(std::piecewise_construct_t(),
											   std::forward_as_tuple(std::move(move), std::move(move)),
											   std::forward_as_tuple(std::move(move), std::move(move)));
	}

	template <typename Type1, typename Type2, typename Obj>
	void constructWithCopy(const Obj& copy)
	{
		trs::CompressedPair<Type1, Type2> pair(std::piecewise_construct_t(),
											   std::forward_as_tuple(copy, copy),
											   std::forward_as_tuple(copy, copy));
	}

	class EmptyType
	{
	public:
		EmptyType() = default;

		EmptyType(const EmptyType& other)
		{
			other.copyProxy();
		}
		EmptyType(EmptyType&& other)
		{
			other.moveProxy();
		}

		void copyProxy() const
		{
			copy();
		}

		void moveProxy()
		{
			move();
		}

		MOCK_CONST_METHOD0(copy, void());
		MOCK_METHOD0(move, void());
	};

	class NonEmptyType
	{
	public:
		NonEmptyType() = default;

		NonEmptyType(const NonEmptyType& other)
		{
			other.copyProxy();
		}
		NonEmptyType(NonEmptyType&& other)
		{
			other.moveProxy();
		}

		void copyProxy() const
		{
			copy();
		}

		void moveProxy()
		{
			move();
		}

		MOCK_CONST_METHOD0(copy, void());
		MOCK_METHOD0(move, void());

		int a = 0;
	};

	class ConstructEmpty
	{
	public:
		ConstructEmpty(EmptyType, EmptyType)
		{
		}
	};

	class ConstructNonEmpty
	{
	public:
		ConstructNonEmpty(EmptyType, EmptyType)
		{
		}

		int a = 0;
	};

	SCENARIO("Compressed pair piecewise construction", "CompressedPair")
	{
		GIVEN("A movable Type")
		{
			EmptyType move;
			EXPECT_CALL(move, move()).Times(4);
			ON_CALL(move, copy()).WillByDefault([] {
				FAIL_CHECK("Should not call copy.");
			});

			WHEN("Constructing a compressed pair EmptyNonEmpty")
			{
				constructWithMove<ConstructEmpty, ConstructNonEmpty>(std::move(move));
				THEN("Type is moved 4 times and copy is never called.")
				{
					testing::Mock::VerifyAndClearExpectations(&move);
				}
			}

			WHEN("Constructing a compressed pair NonEmptyEmpty")
			{
				constructWithMove<ConstructNonEmpty, ConstructEmpty>(std::move(move));
				THEN("Type is moved 4 times and copy is never called.")
				{
					testing::Mock::VerifyAndClearExpectations(&move);
				}
			}

			WHEN("Constructing a compressed pair NonEmptyNonEmpty")
			{
				constructWithMove<ConstructNonEmpty, ConstructNonEmpty>(std::move(move));
				THEN("Type is moved 4 times and copy is never called.")
				{
					testing::Mock::VerifyAndClearExpectations(&move);
				}
			}
		}

		GIVEN("A copyable Type")
		{
			EmptyType copy;
			EXPECT_CALL(copy, copy()).Times(4);
			ON_CALL(copy, move()).WillByDefault([] {
				FAIL_CHECK("Should not call move.");
			});

			WHEN("Constructing a compressed pair EmptyNonEmpty")
			{
				constructWithCopy<ConstructEmpty, ConstructNonEmpty>(copy);
				THEN("Type is copied 4 times and move is never called.")
				{
					testing::Mock::VerifyAndClearExpectations(&copy);
				}
			}

			WHEN("Constructing a compressed pair NonEmptyEmpty")
			{
				constructWithCopy<ConstructNonEmpty, ConstructEmpty>(copy);
				THEN("Type is copied 4 times and move is never called.")
				{
					testing::Mock::VerifyAndClearExpectations(&copy);
				}
			}

			WHEN("Constructing a compressed pair NonEmptyNonEmpty")
			{
				constructWithCopy<ConstructNonEmpty, ConstructNonEmpty>(copy);
				THEN("Type is copied 4 times and move is never called.")
				{
					testing::Mock::VerifyAndClearExpectations(&copy);
				}
			}
		}
	}

	template <typename Type1, typename Type2>
	void constructWithMove(Type1&& move1, Type2&& move2)
	{
		trs::CompressedPair<Type1, Type2> pair(std::move(move1), std::move(move2));
	}

	template <typename Type1, typename Type2>
	void constructWithCopy(const Type1& copy1, const Type2& copy2)
	{
		trs::CompressedPair<Type1, Type2> pair(copy1, copy2);
	}

	SCENARIO("Compressed pair forwarding construction", "CompressedPair")
	{
		GIVEN("A movable Type")
		{
			EmptyType move1;
			ON_CALL(move1, copy()).WillByDefault([] {
				FAIL_CHECK("Should not call copy.");
			});

			NonEmptyType move2;
			ON_CALL(move2, copy()).WillByDefault([] {
				FAIL_CHECK("Should not call copy.");
			});

			WHEN("Constructing a compressed pair EmptyNonEmpty")
			{
				EXPECT_CALL(move1, move()).Times(1);
				EXPECT_CALL(move2, move()).Times(1);
				constructWithMove<EmptyType, NonEmptyType>(std::move(move1), std::move(move2));
				THEN("move1 and move2 are moved 1 time and copy is never called.")
				{
					testing::Mock::VerifyAndClearExpectations(&move1);
					testing::Mock::VerifyAndClearExpectations(&move2);
				}
			}

			WHEN("Constructing a compressed pair NonEmptyEmpty")
			{
				EXPECT_CALL(move1, move()).Times(1);
				EXPECT_CALL(move2, move()).Times(1);
				constructWithMove<NonEmptyType, EmptyType>(std::move(move2), std::move(move1));
				THEN("move1 and move2 are moved 1 time and copy is never called.")
				{
					testing::Mock::VerifyAndClearExpectations(&move1);
					testing::Mock::VerifyAndClearExpectations(&move2);
				}
			}

			WHEN("Constructing a compressed pair NonEmptyNonEmpty")
			{
				EXPECT_CALL(move2, move()).Times(2);
				constructWithMove<NonEmptyType, NonEmptyType>(std::move(move2), std::move(move2));
				THEN("move2 is moved 2 time and copy is never called.")
				{
					testing::Mock::VerifyAndClearExpectations(&move2);
				}
			}
		}

		GIVEN("A copyable Type")
		{
			EmptyType copy1;
			ON_CALL(copy1, move()).WillByDefault([] {
				FAIL_CHECK("Should not call move.");
			});

			NonEmptyType copy2;
			ON_CALL(copy2, move()).WillByDefault([] {
				FAIL_CHECK("Should not call move.");
			});

			WHEN("Constructing a compressed pair EmptyNonEmpty")
			{
				EXPECT_CALL(copy1, copy()).Times(1);
				EXPECT_CALL(copy2, copy()).Times(1);
				constructWithCopy<EmptyType, NonEmptyType>(copy1, copy2);

				THEN("copy1 and copy2 are copied 1 time and move is never called.")
				{
					testing::Mock::VerifyAndClearExpectations(&copy1);
					testing::Mock::VerifyAndClearExpectations(&copy2);
				}
			}

			WHEN("Constructing a compressed pair NonEmptyEmpty")
			{
				EXPECT_CALL(copy1, copy()).Times(1);
				EXPECT_CALL(copy2, copy()).Times(1);
				constructWithCopy<NonEmptyType, EmptyType>(copy2, copy1);
				THEN("copy1 and copy2 are copied 1 time and move is never called.")
				{
					testing::Mock::VerifyAndClearExpectations(&copy1);
					testing::Mock::VerifyAndClearExpectations(&copy2);
				}
			}

			WHEN("Constructing a compressed pair NonEmptyNonEmpty")
			{
				EXPECT_CALL(copy2, copy()).Times(2);
				constructWithCopy<NonEmptyType, NonEmptyType>(copy2, copy2);
				THEN("copy2 is copied 2 time and move is never called.")
				{
					testing::Mock::VerifyAndClearExpectations(&copy2);
				}
			}
		}
	}
}  // namespace Tests