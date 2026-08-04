#pragma once

#include <tuple>

namespace vortex::containers
{
    template <typename... Args>
    using VxTuple = std::tuple<Args...>;
}