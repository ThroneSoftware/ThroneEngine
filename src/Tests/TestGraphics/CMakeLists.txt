include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addTestTarget(TestGraphics)

target_link_libraries(TestGraphics PUBLIC Graphics)