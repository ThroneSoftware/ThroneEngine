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
		class DefaultDeleter
		{
		public:
			void operator()(Type* ptr) noexcept
			{
				delete ptr;
			}
		};

		template <typename Type>
		void setResource(Private::BaseResource<Type>* resource)
		{
			if constexpr(std::is_base_of_v<EnableSharedFromThis<Type>, Type>)
			{
				// Use c-style cast because c++ casts do not enable us to cast to a private base class.
				((PointersPrivate::EnableSharedFromThisPrivate<Type>*)resource->getPtr())->setResource(resource);
			}
		}
	}  // namespace PointersPrivate

	template <typename Type>
	PtrOwner<Type> makePtrOwner(Type* ptr)
	{
		// todo, the ptr will leak with DefaultNotifier.
		auto* resource =
			new Private::SeparatedResource<Type, PointersPrivate::DefaultNotifier<Type>, PointersPrivate::DefaultDeleter<Type>>(
				PointersPrivate::DefaultNotifier<Type>(),
				PointersPrivate::DefaultDeleter<Type>(),
				ptr);
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

	template <typename Type, typename Notifier, typename Deleter>
	PtrOwner<Type> makePtrOwnerWithNotifier(Notifier&& notifier, Deleter&& deleter, Type* ptr)
	{
		auto* resource =
			new Private::SeparatedResource<Type, Notifier, Deleter>(std::forward<Notifier>(notifier), std::forward<Deleter>(deleter), ptr);
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