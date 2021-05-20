include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addSampleTarget(Triangle)

target_link_libraries(Triangle PUBLIC Graphics)