#pragma once

#include <typeindex>
#include <vector>

namespace trc
{
	template <typename... Args>
	class DependenciesTypeList
	{
	public:
		static std::vector<std::type_index> getTypeIndices()
		{
			return {typeid(Args)...};
		}
	};

	class Dependencies
	{
	public:
		Dependencies(std::vector<std::type_index>&& dependencies = {});

		const std::vector<std::type_index>& getDependencies() const;

	private:
		std::vector<std::type_index> m_dependencies;
	};
}  // namespace trc