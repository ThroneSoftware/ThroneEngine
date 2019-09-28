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

function(add_vendor_static_library target_name)
    # Specify a static library
    # [Visual Studio] This will create a project
    add_library(${target_name} STATIC)

    # Specify a directoy to be included in the project source code
    # The subdirectories needs a CMakeLists.txt
    add_subdirectory(Vendors/${target_name})

    # Adds compiler options, at the time of writing this i'm unsure why the PRIVATE is required
    target_compile_options(${target_name} PRIVATE "/permissive-")

    groupTargetSources(${target_name} Vendors/${target_name})
endfunction()

function(add_vendor_external_library target_name)
    find_library(Vendors/${target_name} ${target_name})
endfunction()

function(config_target target_name)
    # Add an include directory to target
    # Thoses are the includes with <>
    target_include_directories(${target_name} PRIVATE "src")

    target_include_directories(${target_name} PRIVATE "Vendors")

    target_include_directories(${target_name} PRIVATE "Vendors/Vendors/Boost/boost_1_71_0/include")

    target_include_directories(${target_name} PRIVATE "Vendors/Vendors/glm/include")

    target_include_directories(${target_name} PRIVATE "Vendors/Vendors/Vulkan/1.1.121.2/include")

    target_include_directories(${target_name} PRIVATE "Vendors/Vendors/gsl/include")

    target_include_directories(${target_name} PRIVATE "Vendors/Vendors/fmt/include")

    target_link_libraries(${target_name} debug "${PROJECT_SOURCE_DIR}/Vendors/Vendors/fmt/Build/Debug/fmtd.lib")
    target_link_libraries(${target_name} optimized "${PROJECT_SOURCE_DIR}/Vendors/Vendors/fmt/Build/Release/fmt.lib")



    # Specify a directoy to be included in the project source code
    # The subdirectories needs a CMakeLists.txt
    add_subdirectory(src/${target_name})

    # Adds compiler options, at the time of writing this i'm unsure why the PRIVATE is required
    target_compile_options(${target_name} PRIVATE "/permissive-")

    groupTargetSources(${target_name} src/${target_name})
endfunction()


function(addStaticLibrary lib_name)
    # Specify a static library
    # [Visual Studio] This will create a project
    add_library(${lib_name} STATIC)

    config_target(${lib_name})
endfunction()

function(addExecutable executable_name)
    # Creates the executable target
    # The empty "" is required even if it's empty
    # [Visual Studio] This will create a project
    add_executable(${executable_name} "")

    config_target(${executable_name})
endfunction()

function(main)
    ## Standard
    addStaticLibrary(Standard)

    ## Utilities
    addStaticLibrary(Utilities)
    target_link_libraries(Utilities Standard)

    ## Core
    addStaticLibrary(Core)
    target_link_libraries(Core Standard Utilities)

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
    target_link_libraries(ManualTesting Graphics Physics Networking Audio)
endfunction()

main()