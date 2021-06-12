#include "Image.h"

namespace trg
{
	Image::Image(const std::string& name, ImageLayout layout, int width, int height, std::vector<uint8_t> data)
	  : m_name(name)
	  , m_layout(layout)
	  , m_width(width)
	  , m_height(height)
	  , m_data(std::move(data))
	{
	}

	std::span<const uint8_t> Image::getData() const
	{
		return m_data;
	}

	ImageLayout Image::getLayout() const
	{
		return m_layout;
	}

	int Image::getWidth() const
	{
		return m_width;
	}

	int Image::getHeight() const
	{
		return m_height;
	}
}  // namespace trg