#vk-bootstrap
include(FetchContent)
FetchContent_Declare(
    fetch_vk_bootstrap
    GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap
    GIT_TAG v0.2
)
FetchContent_MakeAvailable(fetch_vk_bootstrap)
#vk-bootstrap