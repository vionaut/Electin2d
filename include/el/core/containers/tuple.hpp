#pragma once

#include <tuple>

namespace el
{
    template <typename... Args>
    using ElTuple = std::tuple<Args...>;
}