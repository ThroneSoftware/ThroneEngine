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
		using Notifier = PointersPrivate::DefaultNotifier<Type>;
		using Deleter = PointersPrivate::DefaultDeleter<Type>;

		using Resource = Private::SeparatedResource<Type, Notifier, Deleter>;

		auto* resource = new Resource(Notifier(), Deleter(), ptr);
		PointersPrivate::setResource(resource);
		return PtrOwner<Type>(resource);
	}

	template <typename Type, typename... Args>
	PtrOwner<Type> makePtrOwner(Args&&... args)
	{
		using Notifier = PointersPrivate::DefaultNotifier<Type>;

		using Resource = Private::CombinedResource<Type, Notifier>;

		auto* resource = new Resource(Notifier(), std::forward<Args>(args)...);
		PointersPrivate::setResource(resource);
		return PtrOwner<Type>(resource);
	}

	template <typename Type, typename Notifier, typename Deleter>
	PtrOwner<Type> makePtrOwnerWithNotifier(Notifier&& notifier, Deleter&& deleter, Type* ptr)
	{
		using Resource = Private::SeparatedResource<Type, Notifier, Deleter>;

		auto* resource = new Resource(std::forward<Notifier>(notifier), std::forward<Deleter>(deleter), ptr);
		PointersPrivate::setResource(resource);
		return PtrOwner<Type>(resource);
	}

	template <typename Type, typename Notifier, typename... Args>
	PtrOwner<Type> makePtrOwnerWithNotifier(Notifier&& notifier, Args&&... args)
	{
		using Resource = Private::CombinedResource<Type, Notifier>;

		auto* resource = new Resource(std::forward<Notifier>(notifier), std::forward<Args>(args)...);
		PointersPrivate::setResource(resource);
		return PtrOwner<Type>(resource);
	}
}  // namespace trs