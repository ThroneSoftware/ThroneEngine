#pragma once

#include <Vulkan/vulkan.hpp>

#pragma region GLM

#include <glm/common.hpp>
#include <glm/ext.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>

#pragma endregion GLM

#pragma region STD

#include <math.h>

#include <algorithm>
#include <exception>
#include <filesystem>
#include <functional>
#include <limits>
#include <memory>
#include <random>
#include <type_traits>
#include <typeindex>

#pragma region CONCURENCY
#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <shared_mutex>
#include <thread>
#pragma endregion CONCURENCY

#pragma region TIME
#include <chrono>
#include <ctime>
#include <iomanip>
#include <locale>
#include <sstream>
#pragma endregion TIME

#pragma region STREAM
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#pragma endregion STREAM

#pragma region STD_CONTAINERS
#include <any>
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#pragma endregion

#pragma endregion STD

#pragma region FMT
#include <fmt/compile.h>
#include <fmt/format.h>
#pragma endregion FMT

#pragma region GSL
#include <gsl/gsl>
#pragma endregion GSL
