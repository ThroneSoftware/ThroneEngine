#pragma once

#include "../Private/Resource.h"
#include "PtrOwner.h"

namespace trs
{
	namespace PointersPrivate
	{
		template <typename Type>
		class DefaultNotifier
		{
		public:
			void operator()(Type*) noexcept
			{
			}
		};
	}  // namespace PointersPrivate

	template <typename Type>
	PtrOwner<Type> makePtrOwner(Type* ptr)
	{
		auto* resource =
			new Private::SeparatedResource<Type, PointersPrivate::DefaultNotifier<Type>>(PointersPrivate::DefaultNotifier<Type>(), ptr);
		return PtrOwner<Type>(resource);
	}

	template <typename Type, typename... Args>
	PtrOwner<Type> makePtrOwner(Args&&... args)
	{
		auto* resource =
			new Private::CombinedResource<Type, PointersPrivate::DefaultNotifier<Type>>(PointersPrivate::DefaultNotifier<Type>(),
																						std::forward<Args>(args)...);
		return PtrOwner<Type>(resource);
	}

	template <typename Type, typename Notifier>
	PtrOwner<Type> makePtrOwnerWithNotifier(Notifier&& notifier, Type* ptr)
	{
		auto* resource = new Private::SeparatedResource<Type, Notifier>(std::forward<Notifier>(notifier), ptr);
		return PtrOwner<Type>(resource);
	}

	template <typename Type, typename Notifier, typename... Args>
	PtrOwner<Type> makePtrOwnerWithNotifier(Notifier&& notifier, Args&&... args)
	{
		auto* resource = new Private::CombinedResource<Type, Notifier>(std::forward<Notifier>(notifier), std::forward<Args>(args)...);
		return PtrOwner<Type>(resource);
	}
}  // namespace trs