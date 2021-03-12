include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addTestTarget(TestStandard)

target_link_libraries(TestStandard PUBLIC Standard TestsUtilities)