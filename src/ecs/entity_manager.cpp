#include "el/ecs/entity_manager.hpp"
#include "el/core/io/logger/el_logger.hpp"

namespace el
{
    ElEntityManager::ElEntityManager()
        : m_activeEntities(0), m_nextFreeEntity(0)
    {
        m_isActiveFlags.reset();

        for (size_t i = 0; i < m_entities.size() - 1; ++i)
        {
            m_entities[i] = i + 1;
        }

        m_entities.back() = 0;

        EL_LOG_INFO("[Entity Manager] Initialized Succesfully.");
    }

    ElEntity ElEntityManager::create()
    {
        EL_ASSERT((m_activeEntities < el::MAX_ENTITIES), "[EntityManager]: Entity pool exhausted! Maximum of {} entities reached.", el::MAX_ENTITIES);

        ElEntity index = m_nextFreeEntity, gen = getEntityGeneration(m_entities[m_nextFreeEntity]);
        m_nextFreeEntity = getEntityIndex(m_entities[m_nextFreeEntity]);
        m_isActiveFlags.set(index);
        ++m_activeEntities;

        ElEntity out_ent = generateEntity(index, gen);

        EL_LOG_DEBUG("[Entity Manager] Created Entity {}", out_ent);
        return out_ent;
    }

    void ElEntityManager::destroy(ElEntity ent)
    {
        if (!isActive(ent))
        {
            EL_LOG_WARNING("[Entity Manager] Tried to destroy an inactive entity with id: {}", ent);
            return;
        }

        --m_activeEntities;
        ElEntity provided_index = getEntityIndex(ent);
        m_isActiveFlags.reset(provided_index);

        ElEntity provided_generation = getEntityGeneration(m_entities[provided_index]);

        if (provided_generation < el::MAX_ENTITY_GENERATION - (provided_index == getEntityIndex(EL_NULL_ENTITY)))
        {
            ElEntity replace_provided_index = generateEntity(m_nextFreeEntity, provided_generation + 1);
            m_entities[provided_index] = replace_provided_index;

            m_nextFreeEntity = provided_index;
        }

        EL_LOG_DEBUG("[Entity Manager] Destroyed Entity {}", ent);
    }

    bool ElEntityManager::isActive(ElEntity ent)
    {
        ElEntity provided_index = getEntityIndex(ent);
        ElEntity provided_generation = getEntityGeneration(ent);
        return (provided_generation == getEntityGeneration(m_entities[provided_index])) && m_isActiveFlags.test(getEntityIndex(ent));
    }
}