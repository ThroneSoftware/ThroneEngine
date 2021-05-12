#pragma once

namespace tru
{
	template <class... Types>
	struct Overload : Types...
	{
		using Types::operator()...;
	};

	template <class... Types>
	Overload(Types...) -> Overload<Types...>;
}  // namespace tru