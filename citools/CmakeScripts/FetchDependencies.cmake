include(FetchContent)
include(${PROJECT_SOURCE_DIR}/citools/CMakeScripts/TargetConfiguration.cmake)

#vk-bootstrap
FetchContent_Declare(
    fetch_vk_bootstrap
    GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap
    GIT_TAG v0.2
)
FetchContent_MakeAvailable(fetch_vk_bootstrap)

setCompileOptions(vk-bootstrap)
#vk-bootstrap