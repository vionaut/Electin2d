#pragma once

#include <cstdint>
#include "vortex/core/config/vortex_config.hpp"

namespace vortex::ecs
{
    // Uses config:: constants from vortex_config.hpp
    using VxEntity = uint64_t;
    constexpr VxEntity VX_NULL_ENTITY = ~0ULL;

    inline VxEntity getEntityIndex(VxEntity ent)
    {
        return ent & config::ENTITY_INDEX_MASK;
    }

    inline VxEntity getEntityGeneration(VxEntity ent)
    {
        return (ent & config::ENTITY_GENERATION_MASK) >> config::ENTITY_INDEX_BITS;
    }

    inline VxEntity generateEntity(VxEntity index, VxEntity generation)
    {
        return (generation << config::ENTITY_INDEX_BITS) | (index & config::ENTITY_INDEX_MASK);
    }
}