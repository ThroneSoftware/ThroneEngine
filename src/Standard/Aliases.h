#pragma once

#include <pch.h>

namespace trs
{
    template<typename T>
    using vector = std::vector<T>;

    template<typename T>
    using deque = std::deque<T>;

    template<typename T>
    using list = std::list<T>;

    template<typename T>
    using forward_list = std::forward_list<T>;

    template<typename Key, typename Value>
    using unordered_map = std::unordered_map<Key, Value>;

    template<typename Key, typename Value>
    using map = std::map<Key, Value>;

    template<typename T>
    using unordered_set = std::unordered_set<T>;

    template<typename T>
    using set = std::set<T>;
}