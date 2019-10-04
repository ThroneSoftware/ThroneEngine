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
#include <functional>
#include <limits>
#include <memory>
#include <random>
#include <thread>
#include <typeindex>

#pragma region TIME
#include <ctime>
#include <chrono>
#include <iomanip>
#include <locale>
#include <sstream>
#pragma endregion TIME

#pragma region STREAM
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>
#pragma endregion STREAM

#pragma region STD_CONTAINERS
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <vector>
#include <deque>
#include <queue>
#include <list>
#include <string>
#pragma endregion

#pragma endregion STD