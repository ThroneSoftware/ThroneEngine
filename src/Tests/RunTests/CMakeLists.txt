include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetCreation.cmake)

add_executable(RunTests "")

include(FileList.cmake)

setCompileOptions(RunTests)

groupTargetSources(RunTests src/Tests/RunTests)

add_dependencies(RunTests BuildTests)
set_target_properties(RunTests PROPERTIES FOLDER "Tests/Commands")