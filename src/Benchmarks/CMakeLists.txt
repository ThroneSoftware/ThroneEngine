include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

addExecutable(Benchmarks)

target_link_libraries(Benchmarks PUBLIC Core)
target_include_directories(Benchmarks PRIVATE "${PROJECT_SOURCE_DIR}/Vendors/Vendors/nanobench")