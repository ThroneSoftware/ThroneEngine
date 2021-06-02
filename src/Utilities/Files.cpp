#include "Files.h"

#include <fmt/format.h>

namespace tru
{
	void validateFile(const std::filesystem::path& path, std::string_view expectedExtension)
	{
		if(!(path.has_filename() && path.has_extension() && path.extension() == expectedExtension))
		{
			throw std::runtime_error(fmt::format("Path is not a {} file. Path: {}", expectedExtension, path.string()));
		}

		if(!std::filesystem::exists(path))
		{
			throw std::runtime_error(fmt::format("File does not exist. Path: {}", path.string()));
		}
	}
}  // namespace tru