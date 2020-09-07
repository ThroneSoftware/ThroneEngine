#pragma once

#include <pch.h>

#include <Utilities/Utility.h>

namespace trs::Private
{
	template <typename Type>
	class BaseResource
	{
	public:
		using value_type = Type;

	public:
		BaseResource() noexcept = default;

		BaseResource(const BaseResource& other) = delete;
		BaseResource& operator=(const BaseResource& other) = delete;

		BaseResource(BaseResource&& other) = delete;
		BaseResource& operator=(BaseResource&& other) = delete;

		virtual ~BaseResource() noexcept
		{
			assert(m_count == 0 && "Error, deleting a BaseResource while the reference count is higher than 0.");
		}

		void incrementRefCount() noexcept
		{
			++m_count;
		}

		bool incrementRefCountIfNotZero() noexcept
		{
			return tru::incrementNotEqual<0>(m_count);
		}

		void decrementRefCount() noexcept
		{
			assert(m_count != 0);
			if(--m_count == 1)
			{
				notify();
			}
		}

		void incrementWRefCount() noexcept
		{
			++m_wcount;
		}

		bool tryDestroy() noexcept
		{
			if(tru::decrementEqual<1>(m_count))
			{
				destroy();
				tryDestroyCtrlBlock();
				return true;
			}
			else
			{
				return false;
			}
		}

		void tryDestroyCtrlBlock() noexcept
		{
			auto wcount = --m_wcount;
			if(wcount == 0)
			{
				// If --m_wcount returns 0 and m_count is 0
				// then it is impossible to have another reference since m_count == 0 is a final state.
				if(m_count == 0)
				{
					// Resources must always be allocated on the heap.
					delete this;
				}
			}
		}

		virtual value_type* getPtr() noexcept = 0;

	protected:
		/// Notfied is called when m_count goes down to 1 (the last SharedPtr is destroyed).
		virtual void notify() noexcept = 0;

		/// Destroy is used to destroy the object. It is called when m_count == 0.
		virtual void destroy() noexcept = 0;

	private:
		// The PtrOwner and every SharedPtr increment m_count by 1.
		// Possible values:
		// 0  : Final state. It means that the object was destroyed.
		// 1  : Only the PtrOwner exists.
		// >1 : The PtrOwner exists and there is SharedPtrs.
		std::atomic<uint32_t> m_count = 0;

		// The PtrOwner and every WeakPtr increment m_wcount by 1.
		// It is possible for m_wcount to be >=1 even if the PtrOwner and the object are destroyed.
		// Possible values:
		// 0  : Final state. The object is destroyed and the control block will be destroyed.
		// >=1: The PtrOwner or WeakPtrs still exist.
		std::atomic<uint32_t> m_wcount = 0;
	};

	template <typename Type, typename Notifier>
	class CombinedResource : public BaseResource<Type>
	{
	public:
		using value_type = Type;

	public:
		template <typename Notifier, typename... Args>
		CombinedResource(Notifier&& notifier, Args&&... args)
		  : BaseResource<value_type>()
		  , m_notifier(std::forward<Notifier>(notifier))
		{
			new(&m_value) value_type(std::forward<Args>(args)...);
		}

		~CombinedResource() noexcept override
		{
		}

		value_type* getPtr() noexcept override
		{
			return &m_value;
		}

		void notify() noexcept override
		{
			m_notifier(&m_value);
		}

		void destroy() noexcept override
		{
			m_value.~value_type();
		}

	private:
		union
		{
			value_type m_value;
		};
		// todo c++20, use no_unique_address
		Notifier m_notifier;
	};

	template <typename Type, typename Notifier, typename Deleter>
	class SeparatedResource : public BaseResource<Type>
	{
	public:
		using value_type = Type;

	public:
		template <typename Notifier, typename Deleter>
		SeparatedResource(Notifier&& notifier, Deleter&& deleter, value_type* ptr)
		  : BaseResource<value_type>()
		  , m_notifier(std::forward<Notifier>(notifier))
		  , m_deleter(std::forward<Deleter>(deleter))
		  , m_ptr(ptr)
		{
		}

		value_type* getPtr() noexcept override
		{
			return m_ptr;
		}

		void notify() noexcept override
		{
			m_notifier(m_ptr);
		}

		void destroy() noexcept override
		{
			m_deleter(m_ptr);
		}

	private:
		value_type* m_ptr;
		// todo c++20, use no_unique_address
		Notifier m_notifier;
		Deleter m_deleter;
	};
}  // namespace trs::Private