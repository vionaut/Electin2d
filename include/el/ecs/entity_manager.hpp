#pragma once

#include "el/ecs/entity.hpp"
#include "el/core/containers/static_array.hpp"
#include "el/core/config/el_config.hpp"
#include "el/core/containers/bitset.hpp"

namespace el
{
    class ElEntityManager
    {
    public:
        ElEntityManager();
        ~ElEntityManager() = default;

        ElEntity create();
        void destroy(ElEntity ent);
        bool isActive(ElEntity ent);

    private:
        ElEntity m_activeEntities;
        ElEntity m_nextFreeEntity;
        el::ElBitSet<el::MAX_ENTITIES> m_isActiveFlags;
        el::ElStaticArray<ElEntity, el::MAX_ENTITIES> m_entities;
    };
}