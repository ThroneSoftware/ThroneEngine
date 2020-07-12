#include "Dependencies.h"

namespace trc
{
	Dependencies::Dependencies(std::vector<std::type_index>&& dependencies)
	  : m_dependencies(std::move(dependencies))
	{
	}

	const std::vector<std::type_index>& Dependencies::getDependencies() const
	{
		return m_dependencies;
	}
}  // namespace trc