#pragma once

#include <vector>

namespace trg
{
	class Image
	{
	public:
		Image(std::vector<float> data);

	private:
		std::vector<float> m_data;
	};
}  // namespace trg