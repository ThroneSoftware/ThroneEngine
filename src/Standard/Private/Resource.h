#pragma once

#include <pch.h>

namespace trs::Private
{
	template <typename Type>
	class BaseResource
	{
	public:
		using value_type = Type;
        using pointer = value_type*;
        using const_pointer = const value_type*;
		using reference = value_type&;

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
			--m_count;
			if(m_count == 0)
			{
				notify();
			}
		}
		void increaseRefCount() noexcept
		{
			++m_count;
		}

		virtual pointer getPtr() noexcept = 0;
        virtual const_pointer getPtr() const noexcept = 0;

		virtual void notify() const noexcept = 0;

	private:
		std::atomic<uint32_t> m_count = 0;
	};

	template <typename Type>
	class CombinedResource : public BaseResource<Type>
	{
	public:
		using value_type = Type;
		using pointer = value_type*;
        using const_pointer = const value_type*;
		using reference = value_type&;

	public:
		template <typename... Args>
		CombinedResource(Args&&... args)
		  : BaseResource<Type>()
		  , m_value(std::forward<Args>(args)...)
		{
		}

        pointer getPtr() noexcept override
        {
            return &m_value;
        }

        const_pointer getPtr() const noexcept override
		{
            return &m_value;
		}

		void notify() const noexcept override
		{
            assert(false && "Notify not ready!");
		}

	private:
		value_type m_value;
	};
}  // namespace trs::Private