include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addSampleTarget(SimpleWindow)

target_link_libraries(SimpleWindow PUBLIC Graphics)