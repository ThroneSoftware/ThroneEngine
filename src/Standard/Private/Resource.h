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
			assert((m_count == 0 && m_wcount == 0) && "Error, deleting a BaseResource while the reference count is higher than 0.");
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
			auto temp1 = m_count.load();
			auto temp2 = --m_count;
			// Make sure we have no overflow
			assert(temp1 > temp2);
		}

		void incrementWRefCount() noexcept
		{
			++m_wcount;
		}

		bool tryDestroy() noexcept
		{
			if(tru::decrementEqual<1>(m_count))
			{
				resetAllNotified();

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
			if(--m_wcount == 0)
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

		void addNotifiedPtr(gsl::not_null<value_type**> ptr) noexcept
		{
			std::unique_lock lock(m_notifyMutex);

			m_notifyList.emplace_back(ptr);
		}

		void removeNotifiedPtr(gsl::not_null<value_type**> ptr) noexcept
		{
			std::unique_lock lock(m_notifyMutex);

			auto found = std::find(m_notifyList.begin(), m_notifyList.end(), ptr);
			assert(found != m_notifyList.end());
			m_notifyList.erase(found);
		}

		virtual value_type* getPtr() noexcept = 0;

	protected:
		/// Destroy is used to destroy the object. It is called when m_count == 0.
		virtual void destroy() noexcept = 0;

	private:
		void resetAllNotified()
		{
			std::unique_lock lock(m_notifyMutex);

			for(auto ptr: m_notifyList)
			{
				*ptr = nullptr;
			}
			m_notifyList.clear();
		}

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

		std::mutex m_notifyMutex;
		std::vector<value_type**> m_notifyList;
	};

	template <typename Type>
	class CombinedResource : public BaseResource<Type>
	{
	public:
		using value_type = Type;

	public:
		template <typename... Args>
		CombinedResource(Args&&... args)
		  : BaseResource<value_type>()

		{
			new(&m_value) value_type(std::forward<Args>(args)...);
		}

		~CombinedResource() noexcept override
		{
			// no-op destructor required because of the union.
			// destruction of the union member is done by CombinedResource::destroy
		}

		value_type* getPtr() noexcept override
		{
			return &m_value;
		}

		void destroy() noexcept override
		{
			m_value.~value_type();
		}

	private:
		// The union allow us to have unitialized memory to use placement new.
		union
		{
			value_type m_value;
		};
	};

	template <typename Type, typename Deleter>
	class SeparatedResource : public BaseResource<Type>
	{
	public:
		using value_type = Type;

	public:
		template <typename Deleter>
		SeparatedResource(Deleter&& deleter, value_type* ptr)
		  : BaseResource<value_type>()
		  , m_deleter(std::forward<Deleter>(deleter))
		  , m_ptr(ptr)
		{
		}

		value_type* getPtr() noexcept override
		{
			return m_ptr;
		}

		void destroy() noexcept override
		{
			m_deleter(m_ptr);
		}

	private:
		value_type* m_ptr;
		// todo c++20, use no_unique_address
		Deleter m_deleter;
	};
}  // namespace trs::Private