#pragma once

#include "vortex/input/input_backend.hpp"

namespace vortex::input
{
    struct Impl;

    enum class EInputAction
    {
        Up,
        Left,
        Jump,
        Down,
        Right,
        Quit
    };

    class VxInputManager
    {
    public:
        static VxInputManager &getInstance()
        {
            static VxInputManager instance;
            return instance;
        }

        void bindActionKey(EInputAction action, EKeyCode key);

        bool isActionHeld(EInputAction action) const;

        bool isActionPressed(EInputAction action) const;

    private:
        VxInputManager();
        ~VxInputManager();

        Impl *m_impl;
    };
}
