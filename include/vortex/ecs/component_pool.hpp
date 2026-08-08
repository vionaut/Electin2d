#pragma once

#include "vortex/core/containers/vortex_array.hpp"
#include "vortex/ecs/entity_manager.hpp"
#include "vortex/core/config/vortex_config.hpp"
#include "vortex/core/io/logger/vortex_logger.hpp"
#include "vortex/core/io/logger/vortex_formats.hpp"
#include <utility>

namespace vortex::ecs
{
	class IPool
	{
	public:
		virtual ~IPool() = default;

		virtual size_t size() const = 0;

		virtual void remove(VxEntity ent) = 0;

		virtual const vortex::containers::VxArray<VxEntity> &getEntityList() const = 0;
	};

	template <typename T>
	class VxComponentPool : public IPool
	{
	public:
		VxComponentPool()
		{
			VX_LOG_DEBUG("[{}] initializing.", VX_FUNC_SIG);

			m_sparseArray.reserve(config::MAX_ENTITIES);
			m_denseToSparse.reserve(config::MAX_ENTITIES);
			m_denseArray.reserve(config::MAX_ENTITIES);

			for (size_t i = 0; i < config::MAX_ENTITIES; ++i)
			{
				m_sparseArray.push_back(ecs::VX_NULL_ENTITY);
			}

			VX_LOG_INFO("[{}] initialized succesfully.", VX_FUNC_SIG);
		}

		void add(VxEntity ent, const T &component)
		{
			VxEntity index = getEntityIndex(ent);
			if (m_sparseArray[index] == ecs::VX_NULL_ENTITY)
			{
				m_sparseArray[index] = static_cast<VxEntity>(m_denseArray.size());
				m_denseArray.push_back(component);
				m_denseToSparse.push_back(index);

				VX_LOG_DEBUG("Attached {} to entity with id: {}", component, ent);
				return;
			}

			T &old_data = m_denseArray[m_sparseArray[index]];
			VX_LOG_WARNING("Entity with id: {} already has a component: {}, Overriding with new data: {}", ent, old_data, component);
			old_data = std::move(component);

			VX_LOG_DEBUG("Overrode {} to entity with id: {}", component, ent);
		}

		void add(VxEntity ent, T &&component)
		{
			VxEntity index = getEntityIndex(ent);
			if (m_sparseArray[index] == ecs::VX_NULL_ENTITY)
			{
				m_sparseArray[index] = static_cast<VxEntity>(m_denseArray.size());
				m_denseArray.push_back(std::move(component));
				m_denseToSparse.push_back(index);

				VX_LOG_DEBUG("Attached (moved) {} to entity with id: {}", component, ent);

				return;
			}

			T &old_data = m_denseArray[m_sparseArray[index]];
			VX_LOG_WARNING("Entity with id: {} already has a component: {}, Overriding with new data: {}", ent, old_data, component);
			old_data = std::move(component);

			VX_LOG_DEBUG("Overrode (moved) {} to entity with id: {}", component, ent);
		}

		inline bool has(VxEntity ent)
		{
			return m_sparseArray[getEntityIndex(ent)] != ecs::VX_NULL_ENTITY;
		}

		T &get(VxEntity ent)
		{
			VX_ASSERT(has(ent), "Tried to acess uninitialized {} for entity id: {}", VX_FUNC_SIG, ent);

			return m_denseArray[m_sparseArray[getEntityIndex(ent)]];
		}

		const T &get(VxEntity ent) const
		{
			VX_ASSERT(has(ent), "Tried to acess uninitialized {} for entity id: {}", VX_FUNC_SIG, ent);

			return m_denseArray[m_sparseArray[getEntityIndex(ent)]];
		}

		void remove(VxEntity ent) override
		{
			if (!has(ent))
				return;

			VxEntity removed_index = getEntityIndex(ent);
			VxEntity last_index = m_denseToSparse.back();

			if (m_sparseArray[removed_index] != m_sparseArray[last_index])
			{
				m_denseArray[m_sparseArray[removed_index]] = std::move(m_denseArray.back());
				m_denseToSparse[m_sparseArray[removed_index]] = last_index;
				m_sparseArray[last_index] = m_sparseArray[removed_index];
			}

			m_sparseArray[removed_index] = ecs::VX_NULL_ENTITY;

			m_denseArray.pop_back();
			m_denseToSparse.pop_back();

			VX_LOG_DEBUG("{} Succesful.", VX_FUNC_SIG);
		}

		VxComponentPool<T> *castToType(void *ptr)
		{
			return static_cast<VxComponentPool<T> *>(ptr);
		}

		size_t size() const override
		{
			return m_denseArray.size();
		}

		const containers::VxArray<VxEntity> &getEntityList() const override
		{
			return m_denseToSparse;
		}

	private:
		containers::VxArray<VxEntity> m_sparseArray;
		containers::VxArray<VxEntity> m_denseToSparse;
		containers::VxArray<T> m_denseArray;
	};
}