#pragma once

#include "el/ecs/component_pool.hpp"
#include "el/core/containers/tuple.hpp"
#include "el/core/utilities/algorithm.hpp"

#include <iterator>
#include <cstddef>

namespace el
{
    template <typename... Components>
    class ElComponentView
    {
    public:
        ElComponentView(ElComponentPool<Components> *...pools)
            : m_pools(pools...)
        {
            m_isEmpty = ((pools == nullptr) || ...);

            if (!m_isEmpty)
            {
                const IPool *base_pools[] = {static_cast<const IPool *>(pools)...};

                const IPool *min_pool = *minElement(std::begin(base_pools), std::end(base_pools),
                                                           [](const IPool *a, const IPool *b)
                                                           {
                                                               return a->size() < b->size();
                                                           });

                m_targetPool = min_pool;
            }
        }

        class Iterator
        {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = ElEntity;
            using difference_type = std::ptrdiff_t;
            using pointer = ElEntity *;
            using reference = ElEntity &;

            Iterator(const ElComponentView *view, ElEntity index)
                : m_view(view), m_index(index)
            {
                if (m_index < getTargetSize() && !isValid())
                {
                    ++(*this);
                }
            }

            Iterator &operator++()
            {
                do
                {
                    ++m_index;
                } while (m_index < getTargetSize() && !isValid());

                return *this;
            }

            value_type operator*() const
            {
                return m_view->m_targetPool->getEntityList()[m_index];
            }

            friend bool operator==(const Iterator &a, const Iterator &b)
            {
                return (a.m_index == b.m_index) && (a.m_view == b.m_view);
            }

            friend bool operator!=(const Iterator &a, const Iterator &b)
            {
                return (a.m_index != b.m_index) || (a.m_view != b.m_view);
            }

        private:
            size_t getTargetSize() const
            {
                if(m_view->m_targetPool)
                    return m_view->m_targetPool->size();
                return 0;
            }

            bool isValid() const
            {
                ElEntity ent = m_view->m_targetPool->getEntityList()[m_index];
                return m_view->hasAllComponents(ent);
            }

            size_t m_index;
            const ElComponentView *m_view;
        };

        Iterator begin() const
        {
            if (m_isEmpty || m_targetPool->size() == 0)
            {
                return end();
            }
            return Iterator(this, 0);
        }

        Iterator end() const
        {
            return Iterator(this, m_targetPool ? m_targetPool->size() : 0);
        }

    private:
        bool hasAllComponents(ElEntity ent) const
        {
            return apply([ent](ElComponentPool<Components> *...pools)
                                { return (pools->has(ent) && ...); }, m_pools);
        }

        el::ElTuple<ElComponentPool<Components> *...> m_pools;
        const IPool *m_targetPool = nullptr;
        bool m_isEmpty;
    };
}