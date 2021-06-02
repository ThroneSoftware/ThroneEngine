include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addStaticLibrary(Graphics)

target_link_libraries(Graphics PUBLIC Utilities Core)
target_link_libraries(Graphics PRIVATE vk-bootstrap glfw VulkanMemoryAllocator)

target_link_libraries(Graphics PRIVATE debug "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/lib/GLTFSDK.lib") 
target_link_libraries(Graphics PRIVATE optimized "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/GLTFSDK.lib") 

target_compile_definitions(Graphics PRIVATE GLFW_INCLUDE_VULKAN VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 VK_NO_PROTOTYPES)