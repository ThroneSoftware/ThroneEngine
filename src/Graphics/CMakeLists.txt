include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addStaticLibrary(Graphics)

target_link_libraries(Graphics PUBLIC Core)
target_link_libraries(Graphics PRIVATE vk-bootstrap glfw VulkanMemoryAllocator)

target_compile_definitions(Graphics PRIVATE GLFW_INCLUDE_VULKAN VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1)