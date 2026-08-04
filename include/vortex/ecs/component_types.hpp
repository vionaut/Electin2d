#pragma once

#include <cstddef>

namespace vortex::ecs
{
    using ComponentTypeId = size_t;

    inline ComponentTypeId getUniqueId()
    {
        static ComponentTypeId last_id = 0;
        return last_id++;
    }

    template <typename T>
    inline ComponentTypeId getComponentTypeId()
    {
        static const ComponentTypeId type_id = getUniqueId();
        return type_id;
    }
}