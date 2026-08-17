#pragma once

#include "el/core/containers/el_array.hpp"
#include "el/ecs/entity_manager.hpp"
#include "el/core/config/el_config.hpp"
#include "el/core/io/logger/el_logger.hpp"
#include "el/core/io/logger/el_formats.hpp"
#include <utility>
#include <cstddef>

namespace el
{
	class IPool
	{
	public:
		virtual ~IPool() = default;

		virtual size_t size() const = 0;

		virtual void remove(ElEntity ent) = 0;

		virtual const el::ElArray<ElEntity> &getEntityList() const = 0;
	};

	template <typename T>
	class ElComponentPool : public IPool
	{
	public:
		ElComponentPool()
		{
			EL_LOG_DEBUG("[{}] initializing.", EL_FUNC_SIG);

			m_sparseArray.reserve(MAX_ENTITIES);
			m_denseToSparse.reserve(MAX_ENTITIES);
			m_denseArray.reserve(MAX_ENTITIES);

			for (size_t i = 0; i < MAX_ENTITIES; ++i)
			{
				m_sparseArray.push_back(EL_NULL_ENTITY);
			}

			EL_LOG_INFO("[{}] initialized succesfully.", EL_FUNC_SIG);
		}

		void add(ElEntity ent, const T &component)
		{
			ElEntity index = getEntityIndex(ent);
			if (m_sparseArray[index] == EL_NULL_ENTITY)
			{
				m_sparseArray[index] = static_cast<ElEntity>(m_denseArray.size());
				m_denseArray.push_back(component);
				m_denseToSparse.push_back(index);

				EL_LOG_DEBUG("Attached {} to entity with id: {}", component, ent);
				return;
			}

			T &old_data = m_denseArray[m_sparseArray[index]];
			EL_LOG_WARNING("Entity with id: {} already has a component: {}, Overriding with new data: {}", ent, old_data, component);
			old_data = std::move(component);

			EL_LOG_DEBUG("Overrode {} to entity with id: {}", component, ent);
		}

		void add(ElEntity ent, T &&component)
		{
			ElEntity index = getEntityIndex(ent);
			if (m_sparseArray[index] == EL_NULL_ENTITY)
			{
				m_sparseArray[index] = static_cast<ElEntity>(m_denseArray.size());
				m_denseArray.push_back(std::move(component));
				m_denseToSparse.push_back(index);

				EL_LOG_DEBUG("Attached (moved) {} to entity with id: {}", component, ent);

				return;
			}

			T &old_data = m_denseArray[m_sparseArray[index]];
			EL_LOG_WARNING("Entity with id: {} already has a component: {}, Overriding with new data: {}", ent, old_data, component);
			old_data = std::move(component);

			EL_LOG_DEBUG("Overrode (moved) {} to entity with id: {}", component, ent);
		}

		inline bool has(ElEntity ent)
		{
			return m_sparseArray[getEntityIndex(ent)] != EL_NULL_ENTITY;
		}

		T &get(ElEntity ent)
		{
			EL_ASSERT(has(ent), "Tried to acess uninitialized {} for entity id: {}", EL_FUNC_SIG, ent);

			return m_denseArray[m_sparseArray[getEntityIndex(ent)]];
		}

		const T &get(ElEntity ent) const
		{
			EL_ASSERT(has(ent), "Tried to acess uninitialized {} for entity id: {}", EL_FUNC_SIG, ent);

			return m_denseArray[m_sparseArray[getEntityIndex(ent)]];
		}

		void remove(ElEntity ent) override
		{
			if (!has(ent))
				return;

			ElEntity removed_index = getEntityIndex(ent);
			ElEntity last_index = m_denseToSparse.back();

			if (m_sparseArray[removed_index] != m_sparseArray[last_index])
			{
				m_denseArray[m_sparseArray[removed_index]] = std::move(m_denseArray.back());
				m_denseToSparse[m_sparseArray[removed_index]] = last_index;
				m_sparseArray[last_index] = m_sparseArray[removed_index];
			}

			m_sparseArray[removed_index] = EL_NULL_ENTITY;

			m_denseArray.pop_back();
			m_denseToSparse.pop_back();

			EL_LOG_DEBUG("{} Succesful.", EL_FUNC_SIG);
		}

		ElComponentPool<T> *castToType(void *ptr)
		{
			return static_cast<ElComponentPool<T> *>(ptr);
		}

		size_t size() const override
		{
			return m_denseArray.size();
		}

		const ElArray<ElEntity> &getEntityList() const override
		{
			return m_denseToSparse;
		}

	private:
		ElArray<ElEntity> m_sparseArray;
		ElArray<ElEntity> m_denseToSparse;
		ElArray<T> m_denseArray;
	};
}