#pragma once

#include <memory>
#include <utility>

namespace vortex::utils
{
    template <typename T>
    using VxUniquePtr = std::unique_ptr<T>;

    template <typename T>
    using VxLocalSharedPtr = std::shared_ptr<T>;

    template <typename T>
    using VxAtomicSharedPtr = std::shared_ptr<T>;

    template <typename T>
    using VxWeakPtr = std::weak_ptr<T>;

    template <typename T, typename... Args>
    VxUniquePtr<T> vxMakeUnique(Args &&...args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    VxLocalSharedPtr<T> vxMakeLocalShared(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    VxAtomicSharedPtr<T> vxMakeAtomicShared(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}