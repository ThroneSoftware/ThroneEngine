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
			template <typename Type>
			friend bool operator==(const BasePtr<Type>& left, const BasePtr<Type>& right);

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

			BasePtr(std::nullptr_t) noexcept
			  : BasePtr<value_type>()
			{
			}

			BasePtr(Private::BaseResource<value_type>* resource) noexcept
			  : m_resource(resource)
			  , m_ptr(resource->getPtr())
			{
			}

			BasePtr(const BasePtr& other) noexcept
			{
				setWith(other);
			}

			BasePtr& operator=(const BasePtr& other) noexcept
			{
				if(this != &other)
				{
					setWith(other);
				}
				return *this;
			}

			BasePtr(BasePtr&& other) noexcept
			{
				setWith(std::move(other));
			}

			BasePtr& operator=(BasePtr&& other) noexcept
			{
				if(this != &other)
				{
					setWith(std::move(other));
				}
				return *this;
			}

			~BasePtr() noexcept
			{
				reset();
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

			void decreaseRefCount() noexcept
			{
				if(m_resource != nullptr)
				{
					m_resource->decreaseRefCount();
				}
			}

			void increaseRefCount() noexcept
			{
				assert(m_resource != nullptr);
				m_resource->increaseRefCount();
			}

			void destroy() noexcept
			{
				if(m_resource != nullptr)
				{
					delete m_resource;
					reset();
				}
			}

		private:
			template <typename Other>
			void setWith(Other&& other) noexcept
			{
				m_resource = other.m_resource;
				m_ptr = other.m_ptr;

				if constexpr(std::is_rvalue_reference_v<Other&&>)
				{
					other.reset();
				}
			}

			void reset() noexcept
			{
				m_resource = nullptr;
				m_ptr = nullptr;
			}

			Private::BaseResource<value_type>* m_resource;
			pointer m_ptr;
		};

		template <typename Type>
		bool operator==(const BasePtr<Type>& left, const BasePtr<Type>& right)
		{
			return left.m_resource == right.m_resource;
		}

		template <typename Type>
		bool operator!=(const BasePtr<Type>& left, const BasePtr<Type>& right)
		{
			return !(left == right);
		}

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
	class PtrOwner final
	{
		template <typename Type>
		friend class SharedPtr;

	public:
		using value_type = Type;
		using pointer = value_type*;
		using reference = value_type&;

	public:
		PtrOwner(Private::BaseResource<value_type>* resource) noexcept
		  : m_base(resource)
		{
		}

		PtrOwner(const PtrOwner& other) = delete;
		PtrOwner& operator=(const PtrOwner& other) = delete;

		PtrOwner(PtrOwner&& other) noexcept = default;
		PtrOwner& operator=(PtrOwner&& other) noexcept
		{
            // Only need to implement operator= since it is not a use case to construct a PtrOwner with itself.
            // Also, the base cannot be set so it does not need to be destroyed.
			if(this != &other)
			{
				m_base.destroy();
				m_base = std::move(other.m_base);
			}
			return *this;
		}

		~PtrOwner() noexcept
		{
			m_base.destroy();
		}

		pointer getPtr() const noexcept
		{
			return m_base.getPtr();
		}

        pointer operator->() const noexcept
		{
			return m_base.getPtr();
		}

		reference operator*() const noexcept
		{
			return *m_base.getPtr();
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
	class SharedPtr final
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

		SharedPtr(const SharedPtr& other) noexcept
		  : m_base(other.m_base)
		{
			m_base.increaseRefCount();
		}

		SharedPtr& operator=(const SharedPtr& other) noexcept
		{
			if(this != &other)
			{
				m_base.decreaseRefCount();
				m_base = other.m_base;
				m_base.increaseRefCount();
			}
			return *this;
		}

        SharedPtr(SharedPtr&& other) noexcept = default;
		SharedPtr& operator=(SharedPtr&& other) noexcept
		{
            // Only need to implement operator= since it is not a use case to construct a SharedPtr with itself.
            // Also, the base cannot be set so its ref count does not need to be decreased.
			if(this != &other)
			{
				if(m_base != other.m_base)
				{
					m_base.decreaseRefCount();
				}
				m_base = std::move(other.m_base);
			}
			return *this;
		}

		~SharedPtr() noexcept
		{
			m_base.decreaseRefCount();
		}

		pointer getPtr() const noexcept
		{
			return m_base.getPtr();
		}

		pointer operator->() const noexcept
		{
			return m_base.getPtr();
		}

		reference operator*() const noexcept
		{
			return *m_base.getPtr();
		}

	private:
		PointersPrivate::BasePtr<value_type> m_base;
	};
}  // namespace trs