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
    target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/src)

    target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/Vendors)

    target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/Vendors/Vendors/Vulkan/1.2.162.1/include)
endfunction()

function(configTestsIncludeDirectories target_name)
    # Add an include directory to target
    # Thoses are the includes with <>
    target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/Vendors/Vendors/Utility)
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
        target_compile_options(${target_name} PRIVATE "/permissive-" "/W4" "/WX" "/MP") 
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

    include(FileList.cmake)

    setCompileOptions(${target_name})

    groupTargetSources(${target_name} src/${target_name})

    set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
endfunction()

function(configTestTarget target_name)
    configIncludeDirectories(${target_name})
    configTestsIncludeDirectories(${target_name})

    linkCommonVendors(${target_name})
    linkTestVendors(${target_name})

    include(FileList.cmake)

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