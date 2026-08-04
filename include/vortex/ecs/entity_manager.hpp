#pragma once

#include "vortex/ecs/entity.hpp"
#include "vortex/core/containers/static_array.hpp"
#include "vortex/core/config/vortex_config.hpp"
#include "vortex/core/containers/bitset.hpp"

namespace vortex::ecs
{
    class VxEntityManager
    {
    public:

        VxEntityManager();
        ~VxEntityManager() = default;

        VxEntity create();
        void destroy(VxEntity ent);
        bool isActive(VxEntity ent);

    private:
        VxEntity m_activeEntities;
        VxEntity m_nextFreeEntity;
        vortex::containers::VxBitSet<vortex::config::MAX_ENTITIES> m_isActiveFlags;
        vortex::containers::VxStaticArray<VxEntity, vortex::config::MAX_ENTITIES> m_entities;
    };
}