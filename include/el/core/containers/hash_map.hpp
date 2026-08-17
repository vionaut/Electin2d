#pragma once

#include <unordered_map>

namespace el
{
    template <typename K, typename V>
    using ElHashMap = std::unordered_map<K, V>;
}