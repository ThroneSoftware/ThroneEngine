include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addStaticLibrary(VulkanMemoryAllocator)

target_compile_definitions(VulkanMemoryAllocator PUBLIC VMA_STATIC_VULKAN_FUNCTIONS=0)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
target_compile_options(VulkanMemoryAllocator PRIVATE "/wd4100" "/wd4324" "/wd4127") 
else()
message(FATAL_ERROR "Unsupported compiler")
endif()

