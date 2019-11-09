#pragma once

#include <pch.h>

namespace trs
{
	namespace Private
	{
		enum class CompressedPairSwitch
		{
			Disabled,
			EmptyNonEmpty,
			NonEmptyEmpty,
			NonEmptyNonEmpty,
		};

		template <typename Type1, typename Type2>
		constexpr CompressedPairSwitch makeSwitch()
		{
			if constexpr(std::is_empty_v<Type1> && !std::is_final_v<Type1>)
			{
				return CompressedPairSwitch::EmptyNonEmpty;
			}
			else if constexpr(std::is_empty_v<Type2> && !std::is_final_v<Type2>)
			{
				return CompressedPairSwitch::NonEmptyEmpty;
			}
			else
			{
				return CompressedPairSwitch::NonEmptyNonEmpty;
			}
		}

		template <typename Type1, typename Type2, CompressedPairSwitch = CompressedPairSwitch::Disabled>
		class CompressedPairImpl;

		template <typename Type1, typename Type2>
		class CompressedPairImpl<Type1, Type2, CompressedPairSwitch::EmptyNonEmpty> : private Type1
		{
		public:
			constexpr CompressedPairImpl()
			  : Type1()
			  , m_second()
			{
			}

			template <typename Type1, typename Type2>
			constexpr CompressedPairImpl(Type1&& type1, Type2&& type2)
			  : Type1(std::forward<Type1>(type1))
			  , m_second(std::forward<Type2>(type2))
			{
			}

			template <typename... Args1, typename... Args2>
			constexpr CompressedPairImpl(std::piecewise_construct_t, std::tuple<Args1...>&& type1Args, std::tuple<Args2...>&& type2Args)
			  : CompressedPairImpl(std::piecewise_construct_t(),
								   std::move(type1Args),
								   std::move(type2Args),
								   std::make_index_sequence<std::tuple_size_v<std::tuple<Args1...>>>(),
								   std::make_index_sequence<std::tuple_size_v<std::tuple<Args2...>>>())
			{
			}

			constexpr Type1& first() noexcept
			{
				return *this;
			}

			constexpr const Type1& first() const noexcept
			{
				return *this;
			}

			constexpr Type2& second() noexcept
			{
				return m_second;
			}

			constexpr const Type2& second() const noexcept
			{
				return m_second;
			}

		private:
			template <typename... Args1, typename... Args2, std::size_t... Indices1, std::size_t... Indices2>
			constexpr CompressedPairImpl(std::piecewise_construct_t,
										 std::tuple<Args1...>&& type1Args,
										 std::tuple<Args2...>&& type2Args,
										 std::index_sequence<Indices1...>,
										 std::index_sequence<Indices2...>)
			  : Type1(std::get<Indices1>(std::move(type1Args))...)
			  , m_second(std::get<Indices2>(std::move(type2Args))...)
			{
			}

			Type2 m_second;
		};

		template <typename Type1, typename Type2>
		class CompressedPairImpl<Type1, Type2, CompressedPairSwitch::NonEmptyEmpty> : private Type2
		{
		public:
			constexpr CompressedPairImpl()
			  : Type2()
			  , m_first()
			{
			}

			template <typename Type1, typename Type2>
			constexpr CompressedPairImpl(Type1&& type1, Type2&& type2)
			  : Type2(std::forward<Type2>(type2))
			  , m_first(std::forward<Type1>(type1))
			{
			}

			template <typename... Args1, typename... Args2>
			constexpr CompressedPairImpl(std::piecewise_construct_t, std::tuple<Args1...>&& type1Args, std::tuple<Args2...>&& type2Args)
			  : CompressedPairImpl(std::piecewise_construct_t(),
								   std::move(type1Args),
								   std::move(type2Args),
								   std::make_index_sequence<std::tuple_size_v<std::tuple<Args1...>>>(),
								   std::make_index_sequence<std::tuple_size_v<std::tuple<Args2...>>>())
			{
			}

			constexpr Type1& first() noexcept
			{
				return m_first;
			}

			constexpr const Type1& first() const noexcept
			{
				return m_first;
			}

			constexpr Type2& second() noexcept
			{
				return *this;
			}

			constexpr const Type2& second() const noexcept
			{
				return *this;
			}

		private:
			template <typename... Args1, typename... Args2, std::size_t... Indices1, std::size_t... Indices2>
			constexpr CompressedPairImpl(std::piecewise_construct_t,
										 std::tuple<Args1...>&& type1Args,
										 std::tuple<Args2...>&& type2Args,
										 std::index_sequence<Indices1...>,
										 std::index_sequence<Indices2...>)
			  : Type2(std::get<Indices2>(std::move(type2Args))...)
			  , m_first(std::get<Indices1>(std::move(type1Args))...)
			{
			}

			Type1 m_first;
		};

		template <typename Type1, typename Type2>
		class CompressedPairImpl<Type1, Type2, CompressedPairSwitch::NonEmptyNonEmpty>
		{
		public:
			constexpr CompressedPairImpl()
			  : m_first()
			  , m_second()
			{
			}

			template <typename Type1, typename Type2>
			constexpr CompressedPairImpl(Type1&& type1, Type2&& type2)
			  : m_first(std::forward<Type1>(type1))
			  , m_second(std::forward<Type2>(type2))
			{
			}

			template <typename... Args1, typename... Args2>
			constexpr CompressedPairImpl(std::piecewise_construct_t, std::tuple<Args1...>&& type1Args, std::tuple<Args2...>&& type2Args)
			  : CompressedPairImpl(std::piecewise_construct_t(),
								   std::move(type1Args),
								   std::move(type2Args),
								   std::make_index_sequence<std::tuple_size_v<std::tuple<Args1...>>>(),
								   std::make_index_sequence<std::tuple_size_v<std::tuple<Args2...>>>())
			{
			}

			constexpr Type1& first() noexcept
			{
				return m_first;
			}

			constexpr const Type1& first() const noexcept
			{
				return m_first;
			}

			constexpr Type2& second() noexcept
			{
				return m_second;
			}

			constexpr const Type2& second() const noexcept
			{
				return m_second;
			}

		private:
			template <typename... Args1, typename... Args2, std::size_t... Indices1, std::size_t... Indices2>
			constexpr CompressedPairImpl(std::piecewise_construct_t,
										 std::tuple<Args1...>&& type1Args,
										 std::tuple<Args2...>&& type2Args,
										 std::index_sequence<Indices1...>,
										 std::index_sequence<Indices2...>)
			  : m_first(std::get<Indices1>(std::move(type1Args))...)
			  , m_second(std::get<Indices2>(std::move(type2Args))...)
			{
			}

			Type1 m_first;
			Type2 m_second;
		};
	}  // namespace Private

	template <typename Type1, typename Type2>
	class CompressedPair final : public Private::CompressedPairImpl<Type1, Type2, Private::makeSwitch<Type1, Type2>()>
	{
	public:
		using Private::CompressedPairImpl<Type1, Type2, Private::makeSwitch<Type1, Type2>()>::CompressedPairImpl;
	};
}  // namespace trs