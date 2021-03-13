include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addExecutable(ManualTesting)

target_link_libraries(ManualTesting PUBLIC Standard Core Graphics Physics Networking Audio)