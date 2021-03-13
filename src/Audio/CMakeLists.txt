include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addStaticLibrary(Audio)

target_link_libraries(Audio PUBLIC Core)