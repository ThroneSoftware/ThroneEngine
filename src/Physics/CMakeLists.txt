include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addStaticLibrary(Physics)

target_link_libraries(Physics Core)