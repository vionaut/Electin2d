#pragma once

namespace vortex::input
{
    enum class EKeyCode
    {
        Unknown = 0,

        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

        Space, Esc, Enter, Tab, Backspace, Insert, DeleteKey,
        ArrowRight, ArrowLeft, ArrowDown, ArrowUp, PageUp, PageDown, Home, End,
        CapsLock, ScrollLock, NumLock, PrintScreen, PauseKey,
        LeftShift, LeftControl, LeftAlt, LeftSuper,
        RightShift, RightControl, RightAlt, RightSuper,

        Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
        NumpadDecimal, NumpadDivide, NumpadMultiply, NumpadSubtract, NumpadAdd, NumpadEnter, NumpadEqual,

        Apostrophe, Comma, Hyphen, Plus, Period, ForwardSlash, Semicolon, Equal, LeftBracket, Backslash, RightBracket, Tilde
    };

    bool isHardwareKeyPressed(EKeyCode key);
    bool isHardwareKeyDown(EKeyCode key);
}