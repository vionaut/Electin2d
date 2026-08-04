#pragma once

#include <type_traits>
#include <utility>

#include "vortex/core/io/logger/vortex_logger.hpp"

namespace vortex::containers
{
    template <typename T, size_t capacity>
    class VxStaticArray
    {
    public:
        VxStaticArray() : m_size(capacity) {}

        // We can safely allow default copy and move constructors because the data is stored inline!
        VxStaticArray(const VxStaticArray &) = default;
        VxStaticArray(VxStaticArray &&) = default;
        VxStaticArray &operator=(const VxStaticArray &) = default;
        VxStaticArray &operator=(VxStaticArray &&) = default;
        ~VxStaticArray() = default;

        // T &push_back(const T &obj)
        // {
        //     VX_ASSERT(m_size < capacity, "Out of memory!");
        //     m_rawBuffer[m_size] = obj;
        //     ++m_size;
        //     return m_rawBuffer[m_size - 1];
        // }

        // T &push_back(T &&obj)
        // {
        //     VX_ASSERT(m_size < capacity, "Out of memory!");
        //     m_rawBuffer[m_size] = std::move(obj);
        //     ++m_size;
        //     return m_rawBuffer[m_size - 1];
        // }

        // void pop_back()
        // {
        //     VX_ASSERT(m_size > 0, "Empty Array!");
        //     --m_size;
        // }

        // void clear()
        // {
        //     m_size = 0;
        // }

        const T &operator[](size_t i) const
        {
            VX_ASSERT(i < m_size, "index out of range!");
            return m_rawBuffer[i];
        }

        T &operator[](size_t i)
        {
            VX_ASSERT(i < m_size, "index out of range!");
            return m_rawBuffer[i];
        }

        size_t size() const
        {
            return m_size;
        }

        constexpr size_t max_capacity() const
        {
            return capacity;
        }

        // Iterators for range-based for loops
        T *begin() { return &m_rawBuffer[0]; }
        T *end() { return &m_rawBuffer[m_size]; }
        const T *begin() const { return &m_rawBuffer[0]; }
        const T *end() const { return &m_rawBuffer[m_size]; }

        T &back()
        {
            VX_ASSERT(m_size > 0, "Empty Array!");
            return m_rawBuffer[m_size - 1];
        }

        const T &back() const
        {
            VX_ASSERT(m_size > 0, "Empty Array!");
            return m_rawBuffer[m_size - 1];
        }

    private:
        size_t m_size;
        T m_rawBuffer[capacity];
    };
}