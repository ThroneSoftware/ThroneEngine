#pragma once

#include <typeindex>
#include <vector>

namespace trc
{
	class Dependencies
	{
	public:
		Dependencies(std::vector<std::type_index>&& dependencies = {});

		const std::vector<std::type_index>& getDependencies() const;

	private:
		std::vector<std::type_index> m_dependencies;
	};
}  // namespace trc