# Group source files based on their folder
# Remove the prefix relative_source_path
function(groupTargetSources target relative_source_path)
	# get all sources files in a target
	get_target_property(sources ${target} SOURCES)
	foreach(file ${sources})
		# This get the the path of the parent directory in the file
		get_filename_component(parent_directory_path ${file} DIRECTORY)
		
		source_group(TREE 
			${PROJECT_SOURCE_DIR}/${relative_source_path} 
			PREFIX "src\\"
			FILES ${file}
		)
	endforeach()
endfunction()

function(targetLinkLibraryFreetype target_name)
    target_link_libraries(${target_name} freetype)
endfunction()

function(configIncludeDirectories target_name)
    # Add an include directory to target
    # Thoses are the includes with <>
    target_include_directories(${target_name} PRIVATE "src")

    target_include_directories(${target_name} PRIVATE "Vendors")

    target_include_directories(${target_name} PRIVATE "Vendors/Vendors/Vulkan/1.1.121.2/include")
endfunction()

function(configTestsIncludeDirectories target_name)
    # Add an include directory to target
    # Thoses are the includes with <>
    target_include_directories(${target_name} PRIVATE "Vendors/Vendors/Utility")
endfunction()

function(linkCommonVendors target_name)
    target_link_libraries(${target_name} fmt::fmt)
    target_link_libraries(${target_name} Microsoft.GSL::GSL)
    target_link_libraries(${target_name} glm)
endfunction()

function(linkTestVendors target_name)
    target_link_libraries(${target_name} GTest::gtest)
    target_link_libraries(${target_name} GTest::gmock)
    target_link_libraries(${target_name} Catch2::Catch2)
endfunction()

function(setCompileOptions target_name)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # Adds compiler options, at the time of writing this i'm unsure why the PRIVATE is required
        target_compile_options(${target_name} PRIVATE "/permissive-" "/W4" "/WX") 
        # Compile options only in debug mode
        target_compile_options(${target_name} PRIVATE "$<$<CONFIG:DEBUG>:/MTd>") 
        # Compile options only in release mode
        target_compile_options(${target_name} PRIVATE "$<$<CONFIG:RELEASE>:/Oi;/Ot;/GL;/MT>") 
        
        # Adds linker options to release mode
        target_link_options(${target_name} PRIVATE "$<$<CONFIG:RELEASE>:/LTCG:INCREMENTAL>")

    else()
        message(FATAL_ERROR "Unsupported compiler. Add the proper compile options to the specified compiler to activate it.\ 
If you want or need to make it work quick and right now you can comment out the line outputting this message.\n")
    endif()
    
    # Add a #define
    target_compile_definitions(${target_name} PRIVATE GLM_FORCE_SILENT_WARNINGS)
endfunction()

function(configTarget target_name)
    configIncludeDirectories(${target_name})
    linkCommonVendors(${target_name})

    # Specify a directoy to be included in the project source code
    # The subdirectories needs a CMakeLists.txt
    add_subdirectory(src/${target_name})

    setCompileOptions(${target_name})

    groupTargetSources(${target_name} src/${target_name})

    set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
endfunction()

function(configTestTarget target_name)
    configIncludeDirectories(${target_name})
    configTestsIncludeDirectories(${target_name})

    linkCommonVendors(${target_name})
    linkTestVendors(${target_name})
    # Specify a directoy to be included in the project source code
    # The subdirectories needs a CMakeLists.txt
    add_subdirectory(src/Tests/${target_name})

    catch_discover_tests(${target_name})   

    setCompileOptions(${target_name})

    target_compile_definitions(${target_name} PRIVATE GTEST_DONT_DEFINE_FAIL GTEST_DONT_DEFINE_SUCCEED)

    groupTargetSources(${target_name} src/Tests/${target_name})

    add_dependencies(BuildTests ${target_name})

    set_target_properties(${target_name} PROPERTIES FOLDER "Tests")
endfunction()

function(configTargetToUsePch target_name)
    target_precompile_headers(${target_name} REUSE_FROM Pch)
    target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/src/pch)
endfunction()

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


function(addRunTestsTarget)
    add_executable(RunTests "")

    add_subdirectory(src/Tests/RunTests)

    setCompileOptions(RunTests)

    groupTargetSources(RunTests src/Tests/RunTests)

    add_dependencies(RunTests BuildTests)
    set_target_properties(RunTests PROPERTIES FOLDER "Tests/Commands")
endfunction()

function(addPchProj)
    add_library(Pch STATIC)
    configTarget(Pch)
    target_precompile_headers(Pch PRIVATE ${PROJECT_SOURCE_DIR}/src/pch/pch.h)

    setCompileOptions(Pch)

    set_target_properties(Pch PROPERTIES LANGUAGE CXX)
    set_target_properties(Pch PROPERTIES LINKER_LANGUAGE CXX)

    target_include_directories(Pch PRIVATE "${PROJECT_SOURCE_DIR}/Vendors/Vendors/Visual Leak Detector/include")
    target_link_libraries(Pch debug "${PROJECT_SOURCE_DIR}/Vendors/Vendors/Visual Leak Detector/lib/Win64/vld.lib")
endfunction()

function(main)
    ## Precompiled header
    addPchProj()

    ## Standard
    addStaticLibrary(Standard)

    ## Utilities
    addStaticLibrary(Utilities)

    ## Core
    addStaticLibrary(Core)
    target_link_libraries(Core Standard)

    ## Graphics
    addStaticLibrary(Graphics)
    target_link_libraries(Graphics Core)

    ## Physics
    addStaticLibrary(Physics)
    target_link_libraries(Physics Core)

    ## Networking
    addStaticLibrary(Networking)
    target_link_libraries(Networking Core)

    ## Audio
    addStaticLibrary(Audio)
    target_link_libraries(Audio Core)

    ## ManualTesting
    addExecutable(ManualTesting)
    target_link_libraries(ManualTesting Standard Core Graphics Physics Networking Audio)
   
    setupTestProjects()
    
    ## Benchmarks
    addExecutable(Benchmarks)
    target_link_libraries(Benchmarks Core)
    target_include_directories(Benchmarks PRIVATE "${PROJECT_SOURCE_DIR}/Vendors/Vendors/nanobench")

endfunction()

function(setupTestProjects)
    addTestLibrary(TestsUtilities)

    addTestTarget(TestTestFramework)
    addTestTarget(TestStandard)
    target_link_libraries(TestStandard Standard TestsUtilities)
    
    addTestTarget(TestCore)
    target_link_libraries(TestCore Core TestsUtilities)

    addTestTarget(TestUtilities)
    target_link_libraries(TestUtilities TestsUtilities)

    addRunTestsTarget()
endfunction()


main()