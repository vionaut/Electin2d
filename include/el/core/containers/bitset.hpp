#pragma once

#include <bitset>
#include <cstddef>

namespace el
{
    template <size_t T>
    using ElBitSet = std::bitset<T>;
}