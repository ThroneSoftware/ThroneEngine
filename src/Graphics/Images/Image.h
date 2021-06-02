#pragma once

#include <string>
#include <vector>

namespace trg
{
	class Image
	{
	public:
		Image(const std::string& name, std::vector<uint8_t> data);

	private:
		std::string m_name;

		std::vector<uint8_t> m_data;
	};
}  // namespace trg