include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addTestTarget(TestUtilities)

target_link_libraries(TestUtilities TestsUtilities)