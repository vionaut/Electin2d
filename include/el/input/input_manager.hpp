#pragma once

#include "el/input/input_backend.hpp"

namespace el
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

    class ElInputManager
    {
    public:
        static ElInputManager &getInstance()
        {
            static ElInputManager instance;
            return instance;
        }

        void bindActionKey(EInputAction action, EKeyCode key);

        bool isActionHeld(EInputAction action) const;

        bool isActionPressed(EInputAction action) const;

    private:
        ElInputManager();
        ~ElInputManager();

        Impl *m_impl;
    };
}
