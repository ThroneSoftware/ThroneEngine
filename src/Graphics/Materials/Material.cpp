#include "Material.h"

namespace trg
{
	Material::Material(const std::string& name, glm::vec4 baseColorFactor)
	  : m_name(name)
	  , m_baseColorFactor(baseColorFactor)

	{
	}

	void Material::setBaseColorTexture(std::unique_ptr<Image> baseColorTexture)
	{
		m_baseColorTexture = std::move(baseColorTexture);
	}
}  // namespace trg