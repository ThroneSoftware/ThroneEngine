cmake_minimum_required(VERSION 3.14)

# Group source files based on their folder
# Remove the prefix relative_source_path
function(group_target_sources target relative_source_path)
	# get all sources files in a target
	get_target_property(sources ${target} SOURCES)
	foreach(file ${sources})
		# This get the the path of the parent directory in the file
		get_filename_component(parent_directory_path ${file} DIRECTORY)
		
		source_group(TREE 
			${PROJECT_SOURCE_DIR}/${relative_source_path} 
			PREFIX "src\\"
			FILES ${file}
		)
	endforeach()
endfunction()

# Separate the cmake projects(ALL_BUILD and ZERO_CHECK) and the actual projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set the language standard version
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 17)

# Add a project and set it's language to c++
# [Visual Studio] This will create the solution named ThroneEngine
project(ThroneEngine CXX)

# Add a custom target for the custom command
# We make it depends on a DUMMYFILE that never exists so it runs every time we build the project
add_custom_target(GenerateFileList DEPENDS "DUMMYFILE.h")

# Add a command to call GenerateFileList.py at the pre_build event
add_custom_command(TARGET GenerateFileList 
    PRE_BUILD
    COMMAND python
    ${PROJECT_SOURCE_DIR}/src/Core/GenerateFileList.py
)

add_custom_command(TARGET GenerateFileList 
    PRE_BUILD
    COMMAND python
    ${PROJECT_SOURCE_DIR}/src/ManualTesting/GenerateFileList.py
)

set_target_properties(GenerateFileList
	PROPERTIES
	FOLDER "GenerateFileList"
)

# Add a command to rerun ZERO_CHECK after generating files
# I wish we could manualy change dependencies of ZERO_CHECK to make it depend on our projects, but it does
# not seem to be possible.
add_custom_command(TARGET GenerateFileList 
	POST_BUILD 
	COMMAND cmake --build . --target ZERO_CHECK
)

# Sets the output directory for different build outputs (exe, dll, lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/lib)

# Specify a static library
# [Visual Studio] This will create a project
add_library(Core STATIC)

# Specify a directoy to be included in the project source code
# The subdirectories needs a CMakeLists.txt
add_subdirectory(src/Core)

# Adds compiler options, at the time of writing this i'm unsure why the PRIVATE is required
target_compile_options(Core PRIVATE "/permissive-")

group_target_sources(Core src/Core)

# Creates the executable target
# The empty "" is required even if it's empty
# [Visual Studio] This will create a project
add_executable(ManualTesting "")

# Add an include directory to ManualTesting
# Thoses are the includes with <>
include_directories(src)

# Specify a directoy to be included in the project source code
add_subdirectory(src/ManualTesting)

# Specify that a target links with other targets
# This implies add_dependencies
target_link_libraries(ManualTesting Core2)

# Adds compiler options, at the time of writing this i'm unsure why the PRIVATE is required
target_compile_options(ManualTesting PRIVATE "/permissive-")

group_target_sources(ManualTesting src/ManualTesting)