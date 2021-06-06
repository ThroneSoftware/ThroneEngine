#pragma once

#include <span>
#include <string>
#include <vector>

namespace trg
{
	class Image
	{
	public:
		Image(const std::string& name, int width, int height, std::vector<uint8_t> data);

		std::span<const uint8_t> getData() const;
		int getWidth() const;
		int getHeight() const;

	private:
		std::string m_name;

		int m_width = 0;
		int m_height = 0;

		std::vector<uint8_t> m_data;
	};
}  // namespace trg