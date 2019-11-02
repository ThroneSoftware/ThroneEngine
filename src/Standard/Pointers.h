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

			BasePtr(const BasePtr& other) noexcept
			{
				reset(other);
			}

			BasePtr& operator=(const BasePtr& other) noexcept
			{
				if(this != &other)
				{
					reset(other);
				}
			}

			BasePtr(BasePtr&& other) noexcept
			{
				reset(std::move(other));
			}

			BasePtr& operator=(BasePtr&& other) noexcept
			{
				if(this != &other)
				{
					reset(std::move(other));
				}
			}

			~BasePtr() noexcept
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

            void destroy() noexcept
            {
                if (m_resource != nullptr)
                {
					delete m_resource;
                    resetBasePtr(*this);
                }
            }

		private:
            template <typename Other>
            void reset(Other&& other) noexcept
            {
                if (this != &other)
                {
                    resetImpl(std::forward<Other>(other));
                }
            }

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

			void resetBasePtr(BasePtr& value) noexcept
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
			void operator()(Type*) noexcept
			{
			}
		};

	}  // namespace PointersPrivate

	template <typename Type>
	class PtrOwner final
	{
		template <typename Type>
		friend class PtrShared;


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
        PtrOwner& operator=(const PtrOwner & other) = delete;

        PtrOwner(PtrOwner && other) = default;
        PtrOwner& operator=(PtrOwner && other) = default;

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
	class PtrShared final
	{
	public:
		using value_type = Type;
		using pointer = value_type*;
		using reference = value_type&;

	public:
		PtrShared() noexcept
		  : m_base(nullptr)
		{
		}

		explicit PtrShared(const PtrOwner<value_type>& ptrOwner) noexcept
		  : m_base(ptrOwner.m_base)
		{
			m_base.increaseRefCount();
		}

		PtrShared(const PtrShared& other) noexcept
		  : m_base(nullptr)
		{
			m_base.decreaseRefCount();
			m_base.reset(other);
			m_base.increaseRefCount();
		}

		PtrShared& operator=(const PtrShared& other) noexcept
		{
			if(*this != &other)
			{
				m_base.decreaseRefCount();
				m_base.reset(other);
				m_base.increaseRefCount();
			}
		}

		PtrShared(PtrShared&& other) noexcept
		  : m_base(nullptr)
		{
			m_base.decreaseRefCount();
			m_base.reset(other);
		}

		PtrShared& operator=(PtrShared&& other) noexcept
		{
			if(this != &other)
			{
				m_base.decreaseRefCount();
				m_base.reset(other);
			}
		}

		~PtrShared() noexcept
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