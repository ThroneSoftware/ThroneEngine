SET(VCPKG_ROOT_FOLDER "C:/vcpkg" CACHE FILEPATH "The filepath to the root folder of your vcpkg.")

function(vcpkg_install_packages)
    execute_process(
        COMMAND ${VCPKG_ROOT_FOLDER}/vcpkg install ${ARGN} --triplet ${VCPKG_TARGET_TRIPLET} --clean-after-build)
endfunction()

function(install_and_find_packages packages)
    vcpkg_install_packages(${packages})
    foreach(package ${packages})
        find_package(${package} CONFIG REQUIRED)
    endforeach()
endfunction()

function(install_and_find_non_generic_packages)
    # Microsoft GSL
    vcpkg_install_packages(ms-gsl)
    find_package(Microsoft.GSL CONFIG REQUIRED)


endfunction()

function(install_and_find_components module packages components)
    vcpkg_install_packages(${packages})
    find_package(${module} REQUIRED COMPONENTS ${components})
endfunction()




if(VCPKG_ROOT_FOLDER)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        SET(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "Vcpkg target triplet")
    endif()
      
    include(${VCPKG_ROOT_FOLDER}/scripts/buildsystems/vcpkg.cmake)
    
    # vcpkg packages
    set(vcpkg_package_dependencies Catch2;glm;GTest;fmt;Freetype;Stb)
    install_and_find_packages("${vcpkg_package_dependencies}")

    install_and_find_non_generic_packages()

    # vcpkg packages
    set(vcpkg_module_dependencies boost-container;boost-signals2;boost-algorithm)
    # Only libraries (header only should not be in there)
    set(module_components container;)
    install_and_find_components(Boost "${vcpkg_module_dependencies}" "${module_components}")
endif()