#pragma once

#include <pch.h>

#include "Private/Resource.h"

namespace trs
{
	namespace PointersPrivate
	{
		template <typename Type>
		class BasePtr
		{
		public:
			using value_type = Type;
			using pointer = value_type*;
			using reference = value_type&;

		public:
			BasePtr() noexcept
			  : m_resource(nullptr)
			  , m_ptr(nullptr)
			{
			}

			BasePtr(Private::BaseResource<value_type>* resource) noexcept
			  : m_resource(resource)
			  , m_ptr(resource->getPtr())
			{
			}

			BasePtr(const BasePtr& other)
			{
				reset(other);
			}

			BasePtr& operator=(const BasePtr& other)
			{
				if(this != &other)
				{
					reset(other);
				}
			}

			BasePtr(BasePtr&& other)
			{
				reset(std::move(other));
			}

			BasePtr& operator=(BasePtr&& other)
			{
				if(this != &other)
				{
					reset(std::move(other));
				}
			}

			~BasePtr()
			{
				resetBasePtr(*this);
			}

			pointer getPtr() const noexcept
			{
				return m_ptr;
			}

			pointer operator->() const noexcept
			{
				return getPtr();
			}

			reference operator*() const noexcept
			{
				return *getPtr();
			}

			template <typename Other>
			void reset(Other&& other)
			{
				if(this != &other)
				{
					resetImpl(std::forward<Other>(other));
				}
			}

			void decreaseRefCount() noexcept
			{
				assert(m_resource != nullptr);
				if(m_resource != nullptr)
				{
					m_resource->decreaseRefCount();
				}
			}

			void increaseRefCount() noexcept
			{
				assert(m_resource != nullptr);
				if(m_resource != nullptr)
				{
					m_resource->increaseRefCount();
				}
			}

		private:
			void resetImpl(const BasePtr& other) noexcept
			{
				m_resource = other.m_resource;
				m_ptr = other.m_ptr;
			}

			void resetImpl(BasePtr&& other) noexcept
			{
				m_resource = other.m_resource;
				m_ptr = other.m_ptr;

				resetBasePtr(other);
			}

			void resetBasePtr(BasePtr& value)
			{
				value.m_resource = nullptr;
				value.m_ptr = nullptr;
			}

			Private::BaseResource<value_type>* m_resource;
			pointer m_ptr;
		};

		template <typename Type>
		class DefaultNotifier
		{
		public:
			void operator()(Type*)
			{
			}
		};

	}  // namespace PointersPrivate

	template <typename Type>
	class PtrOwner
	{
		template <typename Type>
		friend class SharedPtr;


	public:
		using value_type = Type;
		using pointer = value_type*;
		using reference = value_type&;

	public:
		PtrOwner(Private::BaseResource<value_type>* resource)
		  : m_base(resource)
		{
		}

	private:
		PointersPrivate::BasePtr<value_type> m_base;
	};

	template <typename Type, typename... Args>
	PtrOwner<Type> makePtrOwner(Args&&... args)
	{
		auto* resource =
			new Private::CombinedResource<Type, PointersPrivate::DefaultNotifier<Type>>(PointersPrivate::DefaultNotifier<Type>(),
																						std::forward<Args>(args)...);
		return PtrOwner<Type>(resource);
	}

	template <typename Type, typename Notifier, typename... Args>
	PtrOwner<Type> makePtrOwnerWithNotifier(Notifier&& notifier, Args&&... args)
	{
		auto* resource = new Private::CombinedResource<Type, Notifier>(std::forward<Notifier>(notifier), std::forward<Args>(args)...);
		return PtrOwner<Type>(resource);
	}

	template <typename Type>
	class SharedPtr
	{
	public:
		using value_type = Type;
		using pointer = value_type*;
		using reference = value_type&;

	public:
		SharedPtr() noexcept
		  : m_base(nullptr)
		{
		}

		explicit SharedPtr(const PtrOwner<value_type>& ptrOwner) noexcept
		  : m_base(ptrOwner.m_base)
		{
			m_base.increaseRefCount();
		}

		SharedPtr(const SharedPtr& other)
		  : m_base(nullptr)
		{
			m_base.decreaseRefCount();
			m_base.reset(other);
			m_base.increaseRefCount();
		}

		SharedPtr& operator=(const SharedPtr& other)
		{
			if(*this != &other)
			{
				m_base.decreaseRefCount();
				m_base.reset(other);
				m_base.increaseRefCount();
			}
		}

		SharedPtr(SharedPtr&& other)
		  : m_base(nullptr)
		{
			m_base.decreaseRefCount();
			m_base.reset(other);
		}

		SharedPtr& operator=(SharedPtr&& other)
		{
			if(this != &other)
			{
				m_base.decreaseRefCount();
				m_base.reset(other);
			}
		}

		~SharedPtr() noexcept
		{
			m_base.decreaseRefCount();
		}

	private:
		PointersPrivate::BasePtr<value_type> m_base;
	};
}  // namespace trs