include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

add_library(Pch STATIC)
configTarget(Pch)
target_precompile_headers(Pch PRIVATE ${PROJECT_SOURCE_DIR}/src/pch/pch.h)

setCompileOptions(Pch)

set_target_properties(Pch PROPERTIES LANGUAGE CXX)
set_target_properties(Pch PROPERTIES LINKER_LANGUAGE CXX)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
target_include_directories(Pch PRIVATE "${PROJECT_SOURCE_DIR}/Vendors/Vendors/Visual Leak Detector/include")
target_link_libraries(Pch debug "${PROJECT_SOURCE_DIR}/Vendors/Vendors/Visual Leak Detector/lib/Win64/vld.lib")
endif()