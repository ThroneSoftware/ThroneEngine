include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addTestTarget(TestCore)

target_link_libraries(TestCore PUBLIC Core TestsUtilities)
