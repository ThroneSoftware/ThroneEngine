include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addStaticLibrary(Networking)

target_link_libraries(Networking Core)