include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addStaticLibrary(Graphics)

target_link_libraries(Graphics PUBLIC Core)
target_link_libraries(Graphics PRIVATE vk-bootstrap glfw)