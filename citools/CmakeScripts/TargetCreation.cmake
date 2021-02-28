include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetConfiguration.cmake)

function(addStaticLibrary lib_name)
    # Specify a static library
    # [Visual Studio] This will create a project
    add_library(${lib_name} STATIC)

    configTarget(${lib_name})

    configTargetToUsePch(${lib_name})
endfunction()

function(addExecutable executable_name)
    # Creates the executable target
    # The empty "" is required even if it's empty
    # [Visual Studio] This will create a project
    add_executable(${executable_name} "")

    configTarget(${executable_name})

    configTargetToUsePch(${executable_name})
endfunction()

function(addTestTarget target_name)
    add_executable(${target_name} "")

    configTestTarget(${target_name})

    configTargetToUsePch(${target_name})
endfunction()

function(addTestLibrary target_name)
    add_library(${target_name})

    configTestTarget(${target_name})

    configTargetToUsePch(${target_name})
endfunction()