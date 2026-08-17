#pragma once

#include "el/core/containers/static_array.hpp"
#include "el/core/config/el_config.hpp"

#include "el/ecs/entity_manager.hpp"
#include "el/ecs/component_pool.hpp"
#include "el/ecs/component_types.hpp"
#include "el/ecs/view.hpp"

#include "el/components/physics_components.hpp"
#include "el/core/utilities/memory.hpp"

#include "el/core/io/logger/el_logger.hpp"

namespace el
{
    class ElRegistry
    {
    public:
        ElRegistry()
        {
            for (size_t i = 0; i < m_componentPools.size(); ++i)
            {
                m_componentPools[i] = nullptr;
            }
        }

        template <typename... Comps>
        ElComponentView<Comps...> getView()
        {
            return ElComponentView<Comps...>(getPool<Comps>()...);
        }

        ElEntity createEntity()
        {
            return m_entityManager.create();
        }

        void destroyEntity(ElEntity ent)
        {
            for (size_t i = 0; i < MAX_COMPONENTS; ++i)
            {
                if (m_componentPools[i])
                {
                    m_componentPools[i]->remove(ent);
                }
            }

            m_entityManager.destroy(ent);
        }

        template <typename T>
        void addComponent(ElEntity ent, const T &comp)
        {
            ensurePool<T>()->add(ent, comp);
        }

        template <typename T>
        void addComponent(ElEntity ent, T &&comp)
        {
            ensurePool<T>()->add(ent, std::move(comp));
        }

        template <typename T>
        void removeComponent(ElEntity ent)
        {
            ElComponentPool<T> *pool_ptr = getPool<T>();
            if (pool_ptr)
            {
                pool_ptr->remove(ent);
                return;
            }

            EL_LOG_WARNING("[Registry] Attempted to {} unitialized component for entity with id: {}", EL_FUNC_SIG, ent);
        }

        template <typename T>
        bool hasComponent(ElEntity ent) const
        {
            ElComponentPool<T> *pool_ptr = getPool<T>();
            if (pool_ptr)
                return pool_ptr->has(ent);

            return false;
        }

        template <typename T>
        T &getComponent(ElEntity ent)
        {
            ElComponentPool<T> *pool_ptr = getPool<T>();
            if (!pool_ptr)
            {
                EL_LOG_CRITICAL("[Registry] Attemtped to {} from unitialized component pool for entity with id: {}", EL_FUNC_SIG, ent);
                ElLogger::flush();

                std::abort();
            }
            return pool_ptr->get(ent);
        }

        template <typename T>
        const T &getComponent(ElEntity ent) const
        {
            ElComponentPool<T> *pool_ptr = getPool<T>();
            if (!pool_ptr)
            {
                EL_LOG_CRITICAL("[Registry] Attemtped to {} from unitialized component pool for entity with id: {}", EL_FUNC_SIG, ent);
                ElLogger::flush();

                std::abort();
            }
            return pool_ptr->get(ent);
        }

    private:
        template <typename T>
        void registerComponent(ComponentTypeId type_id)
        {
            m_componentPools[type_id] = elMakeUnique<ElComponentPool<T>>();
        }

        template <typename T>
        ElComponentPool<T> *ensurePool()
        {
            ComponentTypeId type_id = getComponentTypeId<T>();
            if (!m_componentPools[type_id])
                registerComponent<T>(type_id);

            ElComponentPool<T> *pool_ptr = static_cast<ElComponentPool<T> *>(m_componentPools[type_id].get());

            return pool_ptr;
        }

        template <typename T>
        ElComponentPool<T> *getPool() const
        {
            ComponentTypeId type_id = getComponentTypeId<T>();

            ElComponentPool<T> *pool_ptr = static_cast<ElComponentPool<T> *>(m_componentPools[type_id].get());

            return pool_ptr;
        }

        ElEntityManager m_entityManager;
        ElStaticArray<ElUniquePtr<IPool>, MAX_COMPONENTS> m_componentPools;
    };
}