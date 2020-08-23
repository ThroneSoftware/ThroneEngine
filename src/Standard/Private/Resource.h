#pragma once

#include <pch.h>

#include "Standard/CompressedPair.h"

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

		void decreaseRefCount() noexcept
		{
			assert(m_count != 0);
			if(--m_count == 0)
			{
				notify();
			}
		}

		void increaseRefCount() noexcept
		{
			++m_count;
		}

		virtual value_type* getPtr() noexcept = 0;

		virtual void notify() noexcept = 0;

	private:
		std::atomic<uint32_t> m_count = 0;
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
		  , m_value(std::piecewise_construct_t(),
					std::forward_as_tuple(std::forward<Args>(args)...),
					std::forward_as_tuple(std::forward<Notifier>(notifier)))
		{
		}

		value_type* getPtr() noexcept override
		{
			return get();
		}

		void notify() noexcept override
		{
			m_value.second()(get());
		}

	private:
		value_type* get() noexcept
		{
			return &m_value.first();
		}

		CompressedPair<value_type, Notifier> m_value;
	};

	template <typename Type, typename Notifier>
	class SeparatedResource : public BaseResource<Type>
	{
	public:
		using value_type = Type;

	public:
		template <typename Notifier>
		SeparatedResource(Notifier&& notifier, value_type* value)
		  : BaseResource<value_type>()
		  , m_value(std::piecewise_construct_t(), std::forward_as_tuple(value), std::forward_as_tuple(std::forward<Notifier>(notifier)))
		{
		}

		value_type* getPtr() noexcept override
		{
			return get();
		}

		void notify() noexcept override
		{
			m_value.second()(get());
		}

	private:
		value_type* get() noexcept
		{
			return m_value.first();
		}

		CompressedPair<value_type*, Notifier> m_value;
	};
}  // namespace trs::Private