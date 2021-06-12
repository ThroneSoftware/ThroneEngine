#pragma once

#include <cassert>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

namespace trg
{
	enum class ImageLayout : uint32_t
	{
		RgbaU8,
		RgbU8
	};

	class Image
	{
	public:
		Image(const std::string& name, ImageLayout layout, int width, int height, std::vector<uint8_t> data);

		std::span<const uint8_t> getData() const;
		ImageLayout getLayout() const;
		int getWidth() const;
		int getHeight() const;

	private:
		std::string m_name;

		ImageLayout m_layout;

		int m_width = 0;
		int m_height = 0;

		std::vector<uint8_t> m_data;
	};

	inline ImageLayout getImageLayoutFromChannels(uint32_t channels)
	{
		switch(channels)
		{
			case 3:
				return ImageLayout::RgbU8;
			case 4:
				return ImageLayout::RgbaU8;
			default:
				assert(false);
				throw std::runtime_error("Invalid Range.");
		}
	}
}  // namespace trg