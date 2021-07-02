include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addSampleTarget(Models)

target_link_libraries(Models PUBLIC Graphics)