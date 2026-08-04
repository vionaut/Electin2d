#pragma once

#include "vortex/core/containers/static_array.hpp"
#include "vortex/core/config/vortex_config.hpp"

#include "vortex/ecs/entity_manager.hpp"
#include "vortex/ecs/component_pool.hpp"
#include "vortex/ecs/component_types.hpp"
#include "vortex/ecs/view.hpp"

#include "vortex/components/physics_components.hpp"
#include "vortex/core/utilities/memory.hpp"

#include "vortex/core/io/logger/vortex_logger.hpp"

namespace vortex::ecs
{
    class VxRegistry
    {
    public:
        VxRegistry()
        {
            for (size_t i = 0; i < m_componentPools.size(); ++i)
            {
                m_componentPools[i] = nullptr;
            }
        }

        template <typename... Comps>
        VxComponentView<Comps...> getView()
        {
            return VxComponentView<Comps...>(getPool<Comps>()...);
        }

        VxEntity createEntity()
        {
            return m_entityManager.create();
        }

        void destroyEntity(VxEntity ent)
        {
            for (size_t i = 0; i < config::MAX_COMPONENTS; ++i)
            {
                if (m_componentPools[i])
                {
                    m_componentPools[i]->remove(ent);
                }
            }

            m_entityManager.destroy(ent);
        }

        template <typename T>
        void addComponent(VxEntity ent, const T &comp)
        {
            ensurePool<T>()->add(ent, comp);
        }

        template <typename T>
        void addComponent(VxEntity ent, T &&comp)
        {
            ensurePool<T>()->add(ent, std::move(comp));
        }

        template <typename T>
        void removeComponent(VxEntity ent)
        {
            VxComponentPool<T> *pool_ptr = getPool<T>();
            if (pool_ptr)
            {
                pool_ptr->remove(ent);
                return;
            }

            VX_LOG_WARNING("[Registry] Attempted to {} unitialized component for entity with id: {}", VX_FUNC_SIG, ent);
        }

        template <typename T>
        bool hasComponent(VxEntity ent) const
        {
            VxComponentPool<T> *pool_ptr = getPool<T>();
            if (pool_ptr)
                return pool_ptr->has(ent);

            return false;
        }

        template <typename T>
        T &getComponent(VxEntity ent)
        {
            VxComponentPool<T> *pool_ptr = getPool<T>();
            if(!pool_ptr)
            {
                VX_LOG_CRITICAL("[Registry] Attemtped to {} from unitialized component pool for entity with id: {}", VX_FUNC_SIG, ent);
                io::VxLogger::flush();

                std::abort();
            }
            return pool_ptr->get(ent);
        }

        template <typename T>
        const T &getComponent(VxEntity ent) const
        {
            VxComponentPool<T> *pool_ptr = getPool<T>();
            if(!pool_ptr)
            {
                VX_LOG_CRITICAL("[Registry] Attemtped to {} from unitialized component pool for entity with id: {}", VX_FUNC_SIG, ent);
                io::VxLogger::flush();

                std::abort();
            }
            return pool_ptr->get(ent);
        }

    private:
        template <typename T>
        void registerComponent(ComponentTypeId type_id)
        {
            m_componentPools[type_id] = utils::vxMakeUnique<VxComponentPool<T>>();
        }

        template <typename T>
        VxComponentPool<T> *ensurePool()
        {
            ComponentTypeId type_id = getComponentTypeId<T>();
            if (!m_componentPools[type_id])
                registerComponent<T>(type_id);

            VxComponentPool<T> *pool_ptr = static_cast<VxComponentPool<T> *>(m_componentPools[type_id].get());

            return pool_ptr;
        }

        template <typename T>
        VxComponentPool<T> *getPool() const
        {
            ComponentTypeId type_id = getComponentTypeId<T>();

            VxComponentPool<T> *pool_ptr = static_cast<VxComponentPool<T> *>(m_componentPools[type_id].get());

            return pool_ptr;
        }

        VxEntityManager m_entityManager;
        containers::VxStaticArray<utils::VxUniquePtr<IPool>, config::MAX_COMPONENTS> m_componentPools;
    };
}