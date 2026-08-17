#pragma once

#include <cstdint>
#include "el/core/config/el_config.hpp"

namespace el
{
    // Uses  constants from el_config.hpp
    using ElEntity = uint64_t;
    constexpr ElEntity EL_NULL_ENTITY = ~0ULL;

    inline ElEntity getEntityIndex(ElEntity ent)
    {
        return ent & ENTITY_INDEX_MASK;
    }

    inline ElEntity getEntityGeneration(ElEntity ent)
    {
        return (ent & ENTITY_GENERATION_MASK) >> ENTITY_INDEX_BITS;
    }

    inline ElEntity generateEntity(ElEntity index, ElEntity generation)
    {
        return (generation << ENTITY_INDEX_BITS) | (index & ENTITY_INDEX_MASK);
    }
}