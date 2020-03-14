#pragma once

#include "../Private/Resource.h"

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

			explicit BasePtr(Private::BaseResource<value_type>* resource) noexcept
			  : m_resource(resource)
			  , m_ptr(resource->getPtr())
			{
			}

			BasePtr(const BasePtr& other) noexcept
			{
				setWith(other);
			}

			BasePtr& operator=(std::nullptr_t) noexcept
			{
				reset();
				return *this;
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
	}  // namespace PointersPrivate
}  // namespace trs