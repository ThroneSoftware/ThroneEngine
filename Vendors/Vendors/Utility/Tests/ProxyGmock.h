// This file is a hack until gtest releases a new version with the fix in gmock-actions.h that replaces std::result_of with something else.
// vcpkg only has the package that is released and unfortunately gtest does not release often.

#pragma once

// DO NOT USE.
namespace std
{
    template <typename T>
    using result_of = std::invoke_result<T>;
}

#include <gmock/gmock.h>