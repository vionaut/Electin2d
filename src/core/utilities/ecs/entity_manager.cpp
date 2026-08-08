#include "vortex/ecs/entity_manager.hpp"
#include "vortex/core/io/logger/vortex_logger.hpp"

namespace vortex::ecs
{
    VxEntityManager::VxEntityManager()
        : m_activeEntities(0), m_nextFreeEntity(0)
    {
        m_isActiveFlags.reset();

        for (size_t i = 0; i < m_entities.size() - 1; ++i)
        {
            m_entities[i] = i + 1;
        }

        m_entities.back() = 0;

        VX_LOG_INFO("[Entity Manager] Initialized Succesfully.");
    }

    VxEntity VxEntityManager::create()
    {
        VX_ASSERT((m_activeEntities < vortex::config::MAX_ENTITIES), "[EntityManager]: Entity pool exhausted! Maximum of {} entities reached.", vortex::config::MAX_ENTITIES);

        VxEntity index = m_nextFreeEntity, gen = getEntityGeneration(m_entities[m_nextFreeEntity]);
        m_nextFreeEntity = getEntityIndex(m_entities[m_nextFreeEntity]);
        m_isActiveFlags.set(index);
        ++m_activeEntities;

        VxEntity out_ent = generateEntity(index, gen);

        VX_LOG_DEBUG("[Entity Manager] Created Entity {}", out_ent);
        return out_ent;
    }

    void VxEntityManager::destroy(VxEntity ent)
    {
        if (!isActive(ent))
        {
            VX_LOG_WARNING("[Entity Manager] Tried to destroy an inactive entity with id: {}", ent);
            return;
        }

        --m_activeEntities;
        VxEntity provided_index = getEntityIndex(ent);
        m_isActiveFlags.reset(provided_index);

        VxEntity provided_generation = getEntityGeneration(m_entities[provided_index]);

        if (provided_generation < vortex::config::MAX_ENTITY_GENERATION - (provided_index == getEntityIndex(VX_NULL_ENTITY)))
        {
            VxEntity replace_provided_index = generateEntity(m_nextFreeEntity, provided_generation + 1);
            m_entities[provided_index] = replace_provided_index;

            m_nextFreeEntity = provided_index;
        }

        VX_LOG_DEBUG("[Entity Manager] Destroyed Entity {}", ent);
    }

    bool VxEntityManager::isActive(VxEntity ent)
    {
        VxEntity provided_index = getEntityIndex(ent);
        VxEntity provided_generation = getEntityGeneration(ent);
        return (provided_generation == getEntityGeneration(m_entities[provided_index])) && m_isActiveFlags.test(getEntityIndex(ent));
    }
}