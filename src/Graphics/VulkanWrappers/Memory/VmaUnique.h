#pragma once

#include "VmaAllocator.h"

#include <Standard/TypeTraits.h>

#include <Vendors/VulkanMemoryAllocator/vk_mem_alloc.hpp>

namespace trg::vkwrappers
{
	template <typename T>
	concept IsMemoryResource = trs::IsOneOf<T, vk::Buffer, vk::Image>;

	template <IsMemoryResource Type>
	class VmaUnique
	{
	public:
		VmaUnique(std::nullptr_t = nullptr)
		  : m_value(nullptr)
		  , m_allocation(nullptr)
		{
		}

		VmaUnique(Type&& value, vma::Allocation&& allocation)
		  : m_value(std::move(value))
		  , m_allocation(std::move(allocation))
		{
		}

		VmaUnique(std::pair<Type, vma::Allocation>&& resource)
		  : VmaUnique(std::move(resource.first), std::move(resource.second))
		{
		}

		VmaUnique(const VmaUnique& other) = delete;
		VmaUnique& operator=(const VmaUnique& other) = delete;

		VmaUnique(VmaUnique&& other)
		  : m_value(std::move(other.m_value))
		  , m_allocation(std::move(other.m_allocation))
		{
			other.setNull();
		}

		VmaUnique& operator=(VmaUnique&& other)
		{
			if(this != &other)
			{
				reset();

				m_value = std::move(other.m_value);
				m_allocation = std::move(other.m_allocation);

				other.setNull();
			}
			return *this;
		}

		VmaUnique& operator=(std::nullptr_t)
		{
			reset();
			return *this;
		}

		~VmaUnique()
		{
			reset();
		}

		void reset()
		{
			destroy(m_value, m_allocation);

			setNull();
		}

		Type m_value;
		vma::Allocation m_allocation;

	private:
		void setNull()
		{
			m_value = nullptr;
			m_allocation = nullptr;
		}

		static void destroy(vk::Buffer& buffer, vma::Allocation& allocation)
		{
			g_vmaDefaultAllocator.destroyBuffer(buffer, allocation);
		}

		static void destroy(vk::Image& image, vma::Allocation& allocation)
		{
			g_vmaDefaultAllocator.destroyImage(image, allocation);
		}
	};
}  // namespace trg::vkwrappers