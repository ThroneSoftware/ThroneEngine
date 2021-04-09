cmake_minimum_required(VERSION 3.16)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/Vendors/Catch2)

# Separate the cmake projects(ALL_BUILD and ZERO_CHECK) and the actual projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 0)

# Set the language standard version
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 20)

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
    ${PROJECT_SOURCE_DIR}/citools/GenerateFileList.py
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
	COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR} --target ZERO_CHECK
)

add_custom_target(BuildTests)

set_target_properties(BuildTests PROPERTIES FOLDER "Tests/Commands")

# Sets the output directory for different build outputs (exe, dll, lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/lib)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
file(COPY "${PROJECT_SOURCE_DIR}/Vendors/Vendors/Visual Leak Detector/vld.ini"
     DESTINATION "${PROJECT_BINARY_DIR}")
endif()

include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/SetupVcpkg.cmake)
include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/FetchDependencies.cmake)

include(CTest)
enable_testing()
include(${Catch2_DIR}/Catch.cmake)


include(src/Targets.cmake)

add_subdirectory(${PROJECT_SOURCE_DIR}/Vendors/Vendors/VulkanMemoryAllocator)