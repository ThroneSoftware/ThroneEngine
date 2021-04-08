include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addStaticLibrary(VulkanMemoryAllocator)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
target_compile_options(VulkanMemoryAllocator PRIVATE "/wd4100" "/wd4324" "/wd4127") 
else()
message(FATAL_ERROR "Unsupported compiler")
endif()

