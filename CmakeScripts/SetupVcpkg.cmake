SET(VCPKG_ROOT_FOLDER "C:/vcpkg" CACHE FILEPATH "The filepath to the root folder of your vcpkg.")

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/requirements.txt)

function(vcpkg_install_packages)
    execute_process(
        COMMAND ${VCPKG_ROOT_FOLDER}/vcpkg install @${CMAKE_SOURCE_DIR}/requirements.txt --triplet ${VCPKG_TARGET_TRIPLET} --clean-after-build)
endfunction()

function(find_packages packages)
    foreach(package ${packages})
        find_package(${package} CONFIG REQUIRED)
    endforeach()
endfunction()

function(find_non_generic_packages)
    find_package(Microsoft.GSL CONFIG REQUIRED)

    find_package(Stb REQUIRED)
endfunction()

function(find_components module components)
    find_package(${module} REQUIRED COMPONENTS ${components})
endfunction()


if(VCPKG_ROOT_FOLDER)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        SET(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "Vcpkg target triplet")
    endif()
      
    include(${VCPKG_ROOT_FOLDER}/scripts/buildsystems/vcpkg.cmake)
    
    vcpkg_install_packages()

    # vcpkg packages
    set(packages Catch2;glm;GTest;fmt;Freetype)
    find_packages("${packages}")
    find_non_generic_packages()

    # Only libraries (header only libraries should not be in there)
    set(module_components container)
    find_components(Boost "${module_components}")
endif()