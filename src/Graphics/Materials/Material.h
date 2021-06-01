#pragma once

#include "../Images/Image.h"

#include <glm/glm.hpp>

#include <memory>
#include <string>

namespace trg
{
	class Material
	{
	public:
		Material(const std::string& name, glm::vec4 baseColorFactor);

		void setBaseColorTexture(std::unique_ptr<Image> baseColorTexture);

	private:
		std::string m_name;

		// value in linear space.
		glm::vec4 m_baseColorFactor;
		std::unique_ptr<Image> m_baseColorTexture;
	};
}  // namespace trg