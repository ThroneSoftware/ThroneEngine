cmake_minimum_required(VERSION 3.14)

# Separate the cmake projects(ALL_BUILD and ZERO_CHECK) and the actual projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set the language standard version
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 17)

# Add a project and set it's language to c++
project(Throne CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Add a custom target for the custom command
# We make it depends on a DUMMYFILE that never exists so it runs every time we build the project
add_custom_target(GenerateFileList DEPENDS "DUMMYFILE.h")

# Add a command to call GenerateFileList.py at the pre_build event
add_custom_command(TARGET GenerateFileList 
    PRE_BUILD
    COMMAND python
    "${PROJECT_SOURCE_DIR}/src/GenerateFileList.py"
)

# Add a command to rerun ZERO_CHECK after generating files
# I wish we could manualy change dependencies of ZERO_CHECK to make it depend of our projects, but it does
# not seems to be possible.
add_custom_command(TARGET GenerateFileList 
	POST_BUILD 
	COMMAND cmake --build . --target ZERO_CHECK
)

# Creates the executable target
# The empty "" is required even if it's empty
add_executable(Throne "")

# Adds compiler options, at the time of writing this i'm unsure why the PRIVATE is required
target_compile_options(Throne PRIVATE "/permissive-")

# Add sub directories to the target
# Each subdirectoy needs a CMakeLists.txt
# Each CMakeLists.txt should provide the target_sources
add_subdirectory(src)

# https://gist.github.com/Zeex/5291602
# Group source files based on their folder
function(group_target_sources target)
	get_target_property(sources ${target} SOURCES)
	foreach(file ${sources})
		get_filename_component(path "${file}" ABSOLUTE)
		get_filename_component(path "${path}" PATH)
		if(file MATCHES "${PROJECT_BINARY_DIR}")
			file(RELATIVE_PATH path ${PROJECT_BINARY_DIR} "${path}")
		else()
			file(RELATIVE_PATH path ${PROJECT_SOURCE_DIR} "${path}")
		endif()
		string(REGEX REPLACE "/" "\\\\" win_path "${path}")
		source_group("${win_path}" REGULAR_EXPRESSION "${path}/[^/\\]+\\..*")
	endforeach()
endfunction()

group_target_sources(Throne)