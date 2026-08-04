#pragma once

#include <algorithm>
#include <cmath>
#include "vortex/core/io/logger/vortex_logger.hpp"

namespace vortex::utils
{
    // --- Math & Bounds ---

    template <typename T>
    constexpr const T& min(const T& a, const T& b)
    {
        return std::min(a, b);
    }

    template <typename T>
    constexpr const T& max(const T& a, const T& b)
    {
        return std::max(a, b);
    }

    template <typename T>
    constexpr const T& clamp(const T& v, const T& lo, const T& hi)
    {
        return std::clamp(v, lo, hi);
    }

    template <typename T>
    constexpr T abs(T v)
    {
        return std::abs(v);
    }

    template <typename FrwdIt>
    constexpr FrwdIt minElement(FrwdIt first, FrwdIt last)
    {
        if (first == last)
        {
            VX_LOG_CRITICAL("Tried to access min element of an empty array!");
            vortex::io::VxLogger::flush();

            std::abort();
        }

        return std::min_element(first, last);
    }

    template <typename FrwdIt, typename Compare>
    constexpr FrwdIt minElement(FrwdIt first, FrwdIt last, Compare comp)
    {
        if (first == last)
        {
            VX_LOG_CRITICAL("Tried to access min element of an empty array!");
            vortex::io::VxLogger::flush();

            std::abort();
        }

        return std::min_element(first, last, comp);
    }

    // --- Searching & Sorting ---

    template <typename RandomIt>
    void sort(RandomIt first, RandomIt last)
    {
        std::sort(first, last);
    }

    template <typename RandomIt, typename Compare>
    void sort(RandomIt first, RandomIt last, Compare comp)
    {
        std::sort(first, last, comp);
    }

    template <typename InputIt, typename T>
    InputIt find(InputIt first, InputIt last, const T& value)
    {
        if (first == last)
        {
            VX_LOG_CRITICAL("Tried to access find an element from an empty array!");
            vortex::io::VxLogger::flush();

            std::abort();
        }

        return std::find(first, last, value);
    }

    template <typename InputIt, typename UnaryPredicate>
    InputIt findIf(InputIt first, InputIt last, UnaryPredicate p)
    {
        if (first == last)
        {
            VX_LOG_CRITICAL("Tried to access find an element from an empty array!");
            vortex::io::VxLogger::flush();

            std::abort();
        }

        return std::find_if(first, last, p);
    }

    template <typename F, typename Tuple>
    constexpr decltype(auto) apply(F&& f, Tuple&& t)
    {
        return std::apply(std::forward<F>(f), std::forward<Tuple>(t));
    }

    template<typename... Args>
    auto memcpy(Args&&... args)
    {
        return std::memcpy(std::forward<Args>(args)...);
    }
}