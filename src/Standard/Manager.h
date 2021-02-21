#pragma once

#include "BaseManager.h"
#include "Pointers.h"

#include <vector>

namespace trs
{
	template <typename ObjectType>
	class Manager : public BaseManager
	{
	public:
		using value_type = ObjectType;

	private:
		class Deleter
		{
		public:
			Deleter(Manager& manager)
			  : m_manager(manager)
			{
			}

			void operator()(value_type* ptr)
			{
				m_manager.eraseFromPool(ptr);
			}

		private:
			Manager& m_manager;
		};

	public:
		Manager()
		  : BaseManager(typeid(value_type))
		{
		}

		Manager(const Manager& other) = delete;
		Manager& operator=(const Manager& other) = delete;

		Manager(Manager&& other) = delete;
		Manager& operator=(Manager&& other) = delete;

		~Manager() override
		{
			// Order of destruction is important.
			// It is expected that after m_objects.clear() -> m_pool.size() equals 0
			// because when pointers are destroyed they will call Manager::eraseFromPool
			m_objects.clear();
			assert(m_pool.empty());
			m_pool.clear();
		}

		template <typename... Args>
		void emplace(Args&&... args)
		{
			auto& ref = m_pool.emplace_back(std::forward<Args>(args)...);

			auto ptrOwner = makePtrOwnerWithDeleter<value_type, Deleter>(Deleter(*this), &ref);
			m_objects.emplace_back(std::move(ptrOwner));
		}

		template <typename Pointer = NotifiedPtr<Manager::value_type>, typename FindFunc>
		typename Pointer findIf(FindFunc func) const
		{
			auto find_locked = [func](const PtrOwner<value_type>& owner) {
				// TODO: #88 -> Make this thread safe
				return func(*owner);
			};

			auto found = std::find_if(m_objects.begin(), m_objects.end(), find_locked);
			return found != m_objects.end() ? Pointer(*found) : Pointer(nullptr);
		}

		std::size_t size() const noexcept
		{
			return m_objects.size();
		}

		template <typename Func>
		void process(Func func)
		{
			std::for_each(m_objects.begin(), m_objects.end(), [func](const PtrOwner<value_type>& owner) {
				func(*owner);
			});
		}

		bool erase(value_type& ptr)
		{
			auto found = std::find_if(m_objects.begin(), m_objects.end(), [&ptr](PtrOwner<value_type>& ptrOwner) {
				return ptrOwner.getPtr() == &ptr;
			});

			assert(found != m_objects.end());

			bool destroyed = found->tryDestroy();

			if(destroyed)
			{
				m_objects.erase(found);
			}

			return destroyed;
		}

	private:
		void eraseFromPool(value_type* ptr)
		{
			auto found = std::find_if(m_pool.begin(), m_pool.end(), [ptr](value_type& value) {
				return &value == ptr;
			});

			assert(found != m_pool.end());

			m_pool.erase(found);
		}

		std::vector<PtrOwner<value_type>> m_objects;

		// Temporary until we have a better solution, see issue #39.
		// It should also be replaced by a new type. (for example class Pool)
		std::list<value_type> m_pool;
	};
}  // namespace trs