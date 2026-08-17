#pragma once

#include <memory>
#include <utility>

namespace el
{
    template <typename T>
    using ElUniquePtr = std::unique_ptr<T>;

    template <typename T>
    using ElLocalSharedPtr = std::shared_ptr<T>;

    template <typename T>
    using ElAtomicSharedPtr = std::shared_ptr<T>;

    template <typename T>
    using ElWeakPtr = std::weak_ptr<T>;

    template <typename T, typename... Args>
    ElUniquePtr<T> elMakeUnique(Args &&...args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    ElLocalSharedPtr<T> elMakeLocalShared(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    ElAtomicSharedPtr<T> elMakeAtomicShared(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}