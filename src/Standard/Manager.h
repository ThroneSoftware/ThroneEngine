#pragma once

#include "Pointers.h"

#include <vector>

namespace trs
{
	template <typename ObjectType>
	class Manager
	{
	public:
		using value_type = ObjectType;

	private:
		class Notifier
		{
		public:
			Notifier(Manager& manager)
			  : m_manager(manager)
			{
			}

			void operator()(value_type* ptr)
			{
				m_manager.erase(ptr);
			}

		private:
			Manager& m_manager;
		};

	public:
		Manager() = default;

		Manager(const Manager& other) = delete;
		Manager& operator=(const Manager& other) = delete;

		Manager(Manager&& other) = delete;
		Manager& operator=(Manager&& other) = delete;

		~Manager() = default;

		template <typename... Args>
		void emplace(Args&&... args)
		{
			auto& ref = m_pool.emplace_back(std::forward<Args>(args)...);

			auto ptrOwner = trs::makePtrOwnerWithNotifier<value_type, Notifier>(Notifier(*this), &ref);
			m_objects.emplace_back(std::move(ptrOwner));
		}

		template <typename FindFunc>
		trs::SharedPtr<value_type> find(FindFunc func)
		{
			auto find_locked = [func](trs::PtrOwner<value_type>& owner) {
				// TODO: #88 -> Make this thread safe
				return func(*owner);
			};

			auto found = std::find_if(m_objects.begin(), m_objects.end(), find_locked);
			return found != m_objects.end() ? *found : nullptr;
		}

	private:
		void erase(value_type* ptr)
		{
			{
				auto found = std::find_if(m_objects.begin(), m_objects.end(), [ptr](PtrOwner<value_type>& ptrOwner) {
					return ptrOwner.getPtr() == ptr;
				});

				assert(found != m_objects.end());

				m_objects.erase(found);
			}

			{
				auto found = std::find_if(m_pool.begin(), m_pool.end(), [ptr](value_type& value) {
					return &value == ptr;
				});

				m_pool.erase(found);
			}
		}

		std::vector<PtrOwner<value_type>> m_objects;

		// Temporary until we have a better solution, see issue #39.
		// It should also be replaced by a new type. (for example class Pool)
		std::list<value_type> m_pool;

	public:
		// Only used in unit tests
#ifdef TEST_MANAGER
		const std::vector<PtrOwner<value_type>>& getObjects()
		{
			return m_objects;
		}

		const std::list<value_type>& getPool()
		{
			return m_pool;
		}
#endif	// TEST_MANAGER
	};
}  // namespace trs