# Group source files based on their folder
# Remove the prefix target_path
function(groupTargetSources target target_path)
    # Validate that target_path contains one of the prefixes.
    # This validation must exist because the fail case can be hard to debug.
    if(NOT (
        ("${target_path}" MATCHES "${PROJECT_SOURCE_DIR}/src") OR 
        ("${target_path}" MATCHES "${PROJECT_SOURCE_DIR}/Vendors/Vendors"))
    )
        message(FATAL_ERROR "target_path does not contain any of the supported prefixes.")
    endif()

    # Modify target_path so that it becomes the filter that we want in the editor.
    STRING(REPLACE "${PROJECT_SOURCE_DIR}/src" "" filter ${target_path})
    STRING(REPLACE "${PROJECT_SOURCE_DIR}/Vendors/Vendors" "" filter ${filter})
    STRING(REPLACE "/${target}" "" filter ${filter})
    if (NOT ("${filter}" STREQUAL ""))
        set_target_properties(${target_name} PROPERTIES FOLDER ${filter})
    endif()

    # Get all sources files in a target 
    get_target_property(sources ${target} SOURCES)
    foreach(file ${sources})	
        source_group(TREE 
            ${target_path} 
            PREFIX "src\\"
            FILES ${file}
        )
    endforeach()
endfunction()

function(configIncludeDirectories target_name)
    # Add an include directory to target
    # Thoses are the includes with <>
    target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/src)

    target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/Vendors)

    target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/Vendors/Vendors/Vulkan/1.2.170.0/include)
endfunction()

function(configTestsIncludeDirectories target_name)
    # Add an include directory to target
    # Thoses are the includes with <>
    target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/Vendors/Vendors/Utility)
endfunction()

function(linkCommonVendors target_name)
    target_link_libraries(${target_name} PUBLIC fmt::fmt)
    target_link_libraries(${target_name} PUBLIC Microsoft.GSL::GSL)
    target_link_libraries(${target_name} PUBLIC glm::glm)
endfunction()

function(linkTestVendors target_name)
    target_link_libraries(${target_name} PUBLIC GTest::gtest)
    target_link_libraries(${target_name} PUBLIC GTest::gmock)
    target_link_libraries(${target_name} PUBLIC Catch2::Catch2)
endfunction()

function(setCompileOptions target_name)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # Adds compiler options, at the time of writing this i'm unsure why the PRIVATE is required
        target_compile_options(${target_name} PRIVATE "/permissive-" "/W4" "/MP") 
        # Compile options only in debug mode
        target_compile_options(${target_name} PRIVATE "$<$<CONFIG:DEBUG>:/MTd>") 
        # Compile options only in release mode
        target_compile_options(${target_name} PRIVATE "$<$<CONFIG:RELEASE>:/Oi;/Ot;/GL;/MT>") 
        target_compile_options(${target_name} PRIVATE "$<$<CONFIG:RELWITHDEBINFO>:/Oi;/Ot;/GL;/MT>") 
        
        # Adds linker options to release mode
        target_link_options(${target_name} PRIVATE "$<$<CONFIG:RELEASE>:/LTCG:INCREMENTAL>")
        target_link_options(${target_name} PRIVATE "$<$<CONFIG:RELWITHDEBINFO>:/LTCG:INCREMENTAL>")

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

    groupTargetSources(${target_name} ${CMAKE_CURRENT_SOURCE_DIR})

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

    groupTargetSources(${target_name} ${CMAKE_CURRENT_SOURCE_DIR})

    add_dependencies(BuildTests ${target_name})
endfunction()

function(configTestRunner target_name)
    target_link_libraries(${target_name} PRIVATE CatchRunner)
endfunction()

function(configSampleTarget target_name)
    configTarget(${target_name})
endfunction()

function(configTargetToUsePch target_name)
    target_precompile_headers(${target_name} REUSE_FROM Pch)
    target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/src/pch)
endfunction()