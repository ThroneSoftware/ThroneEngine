#pragma once

#include <Standard/Pointers.h>

#include <boost/signals2/connection.hpp>


namespace trc
{
	class Entity;
	class BaseComponent;

	class SharedComponent
	{
	public:
		SharedComponent(trs::SharedPtr<Entity> topParent, trs::SharedPtr<BaseComponent> component);

		SharedComponent(const SharedComponent& other);
		SharedComponent& operator=(const SharedComponent& other);

		SharedComponent(SharedComponent&& other);
		SharedComponent& operator=(SharedComponent&& other);

		~SharedComponent() = default;

	private:
		void connect();

		trs::SharedPtr<Entity> m_topParent;
		trs::SharedPtr<BaseComponent> m_component;

		boost::signals2::scoped_connection m_topParentChangedSignalConnection;
	};
}  // namespace trc