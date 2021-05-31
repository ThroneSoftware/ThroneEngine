#include "Image.h"

namespace trg
{
	Image::Image(const std::string& name, std::vector<uint8_t> data)
	  : m_name(name)
	  , m_data(std::move(data))
	{
	}
}  // namespace trg