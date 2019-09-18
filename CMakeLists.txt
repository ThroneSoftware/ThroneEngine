cmake_minimum_required(VERSION 3.14)

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
    ${PROJECT_SOURCE_DIR}/GenerateFileList.py
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

include(Targets.cmake)