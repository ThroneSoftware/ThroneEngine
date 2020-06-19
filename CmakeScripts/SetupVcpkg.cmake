SET(VCPKG_ROOT_FOLDER "C:/vcpkg" CACHE FILEPATH "The filepath to the root folder of your vcpkg.")

function(vcpkg_install_packages)
    execute_process(
        COMMAND ${VCPKG_ROOT_FOLDER}/vcpkg install ${ARGN} --triplet ${VCPKG_TARGET_TRIPLET} --clean-after-build)
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
    
    set(vcpkg_package_dependencies Catch2;glm;GTest;fmt;Freetype;ms-gsl;Stb;boost-container;boost-signals2;boost-algorithm)
    vcpkg_install_packages("${vcpkg_package_dependencies}")

    # vcpkg packages
    set(packages Catch2;glm;GTest;fmt;Freetype)
    find_packages("${packages}")
    find_non_generic_packages()

    # Only libraries (header only should not be in there)
    set(module_components container)
    find_components(Boost "${module_components}")
endif()