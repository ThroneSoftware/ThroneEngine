#pragma once

#include "../Private/Resource.h"
#include "EnableSharedFromThis.h"
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

		template <typename Type>
		void setResource(Private::BaseResource<Type>* resource)
		{
			if constexpr(std::is_base_of_v<EnableSharedFromThis<Type>, Type>)
			{
				EnableSharedFromThisSetResourceAccessor<Type>().setResource(resource->getPtr(), resource);
			}
		}
	}  // namespace PointersPrivate

	template <typename Type>
	PtrOwner<Type> makePtrOwner(Type* ptr)
	{
		auto* resource =
			new Private::SeparatedResource<Type, PointersPrivate::DefaultNotifier<Type>>(PointersPrivate::DefaultNotifier<Type>(), ptr);
		PointersPrivate::setResource(resource);
		return PtrOwner<Type>(resource);
	}

	template <typename Type, typename... Args>
	PtrOwner<Type> makePtrOwner(Args&&... args)
	{
		auto* resource =
			new Private::CombinedResource<Type, PointersPrivate::DefaultNotifier<Type>>(PointersPrivate::DefaultNotifier<Type>(),
																						std::forward<Args>(args)...);
		PointersPrivate::setResource(resource);
		return PtrOwner<Type>(resource);
	}

	template <typename Type, typename Notifier>
	PtrOwner<Type> makePtrOwnerWithNotifier(Notifier&& notifier, Type* ptr)
	{
		auto* resource = new Private::SeparatedResource<Type, Notifier>(std::forward<Notifier>(notifier), ptr);
		PointersPrivate::setResource(resource);
		return PtrOwner<Type>(resource);
	}

	template <typename Type, typename Notifier, typename... Args>
	PtrOwner<Type> makePtrOwnerWithNotifier(Notifier&& notifier, Args&&... args)
	{
		auto* resource = new Private::CombinedResource<Type, Notifier>(std::forward<Notifier>(notifier), std::forward<Args>(args)...);
		PointersPrivate::setResource(resource);
		return PtrOwner<Type>(resource);
	}
}  // namespace trs