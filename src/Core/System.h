#include "ISystem.h"

#include <Standard/Manager.h>

namespace trc
{
	namespace SystemPrivate
	{
		template <typename T>
		concept HasUpdate = requires(T t)
		{
			t.update();
		};

		template <typename T>
		concept HasLateUpdate = requires(T t)
		{
			t.lateUpdate();
		};
	}  // namespace SystemPrivate


	template <typename ComponentType>
	class System : public ISystem
	{
	public:
		using value_type = ComponentType;

	public:
		System(trs::Manager<value_type>& manager)
		  : m_manager(manager)
		{
		}

		void update() override
		{
			if constexpr(SystemPrivate::HasUpdate<value_type>)
			{
				m_manager.process([](value_type& component) {
					component.update();
				});
			}
		}

		void lateUpdate() override
		{
			if constexpr(SystemPrivate::HasLateUpdate<value_type>)
			{
				m_manager.process([](value_type& component) {
					component.lateUpdate();
				});
			}
		}

	private:
		trs::Manager<value_type>& m_manager;
	};
}  // namespace trc