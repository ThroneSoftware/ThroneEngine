#pragma once

#include "../Private/Resource.h"

#include <gsl/gsl>

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

		public:
			BasePtr(std::nullptr_t = nullptr) noexcept
			  : m_resource(nullptr)
			  , m_ptr(nullptr)
			{
			}

			explicit BasePtr(gsl::not_null<Private::BaseResource<value_type>*> resource) noexcept
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

			value_type* getPtr() const noexcept
			{
				return m_ptr;
			}

			value_type* operator->() const noexcept
			{
				return getPtr();
			}

			value_type& operator*() const noexcept
			{
				return *getPtr();
			}

			void incrementRefCount() noexcept
			{
				if(m_resource != nullptr)
				{
					m_resource->incrementRefCount();
				}
			}

			bool incrementRefCountIfNotZero() noexcept
			{
				if(m_resource != nullptr)
				{
					return m_resource->incrementRefCountIfNotZero();
				}
				return false;
			}

			void decrementRefCount() noexcept
			{
				if(m_resource != nullptr)
				{
					m_resource->decrementRefCount();
				}
			}

			void incrementWRefCount() noexcept
			{
				if(m_resource != nullptr)
				{
					m_resource->incrementWRefCount();
				}
			}

			// BasePtr should not be used after a call to tryDestroyCtrlBlock.
			void tryDestroyCtrlBlock() noexcept
			{
				if(m_resource != nullptr)
				{
					m_resource->tryDestroyCtrlBlock();
				}
			}

			bool tryDestroy() noexcept
			{
				if(m_resource != nullptr)
				{
					return m_resource->tryDestroy();
				}
				return true;
			}

			void addNotifiedPtr() noexcept
			{
				if(m_resource != nullptr && m_ptr != nullptr)
				{
					m_resource->addNotifiedPtr(&m_ptr);
				}
			}

			void removeNotifiedPtr() noexcept
			{
				if(m_resource != nullptr && m_ptr != nullptr)
				{
					m_resource->removeNotifiedPtr(&m_ptr);
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
			// Mutable because of NotifiedPtr
			mutable value_type* m_ptr;
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