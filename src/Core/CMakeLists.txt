include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addStaticLibrary(Core)

target_link_libraries(Core PUBLIC Standard)