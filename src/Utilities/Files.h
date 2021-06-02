#pragma once

#include <filesystem>
#include <string_view>

namespace tru
{
	void validateFile(const std::filesystem::path& path, std::string_view expectedExtension);
}