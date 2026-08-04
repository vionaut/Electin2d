#include "vortex/input/input_backend.hpp"
#include "raylib.h"

using namespace vortex;

namespace vortex::input
{
    static int translateToRaylibKey(EKeyCode key)
    {
        switch (key)
        {
        case EKeyCode::Unknown: return 0;
        case EKeyCode::A: return KEY_A;
        case EKeyCode::B: return KEY_B;
        case EKeyCode::C: return KEY_C;
        case EKeyCode::D: return KEY_D;
        case EKeyCode::E: return KEY_E;
        case EKeyCode::F: return KEY_F;
        case EKeyCode::G: return KEY_G;
        case EKeyCode::H: return KEY_H;
        case EKeyCode::I: return KEY_I;
        case EKeyCode::J: return KEY_J;
        case EKeyCode::K: return KEY_K;
        case EKeyCode::L: return KEY_L;
        case EKeyCode::M: return KEY_M;
        case EKeyCode::N: return KEY_N;
        case EKeyCode::O: return KEY_O;
        case EKeyCode::P: return KEY_P;
        case EKeyCode::Q: return KEY_Q;
        case EKeyCode::R: return KEY_R;
        case EKeyCode::S: return KEY_S;
        case EKeyCode::T: return KEY_T;
        case EKeyCode::U: return KEY_U;
        case EKeyCode::V: return KEY_V;
        case EKeyCode::W: return KEY_W;
        case EKeyCode::X: return KEY_X;
        case EKeyCode::Y: return KEY_Y;
        case EKeyCode::Z: return KEY_Z;
        case EKeyCode::Num0: return KEY_ZERO;
        case EKeyCode::Num1: return KEY_ONE;
        case EKeyCode::Num2: return KEY_TWO;
        case EKeyCode::Num3: return KEY_THREE;
        case EKeyCode::Num4: return KEY_FOUR;
        case EKeyCode::Num5: return KEY_FIVE;
        case EKeyCode::Num6: return KEY_SIX;
        case EKeyCode::Num7: return KEY_SEVEN;
        case EKeyCode::Num8: return KEY_EIGHT;
        case EKeyCode::Num9: return KEY_NINE;
        case EKeyCode::F1: return KEY_F1;
        case EKeyCode::F2: return KEY_F2;
        case EKeyCode::F3: return KEY_F3;
        case EKeyCode::F4: return KEY_F4;
        case EKeyCode::F5: return KEY_F5;
        case EKeyCode::F6: return KEY_F6;
        case EKeyCode::F7: return KEY_F7;
        case EKeyCode::F8: return KEY_F8;
        case EKeyCode::F9: return KEY_F9;
        case EKeyCode::F10: return KEY_F10;
        case EKeyCode::F11: return KEY_F11;
        case EKeyCode::F12: return KEY_F12;
        case EKeyCode::Space: return KEY_SPACE;
        case EKeyCode::Esc: return KEY_ESCAPE;
        case EKeyCode::Enter: return KEY_ENTER;
        case EKeyCode::Tab: return KEY_TAB;
        case EKeyCode::Backspace: return KEY_BACKSPACE;
        case EKeyCode::Insert: return KEY_INSERT;
        case EKeyCode::DeleteKey: return KEY_DELETE;
        case EKeyCode::ArrowRight: return KEY_RIGHT;
        case EKeyCode::ArrowLeft: return KEY_LEFT;
        case EKeyCode::ArrowDown: return KEY_DOWN;
        case EKeyCode::ArrowUp: return KEY_UP;
        case EKeyCode::PageUp: return KEY_PAGE_UP;
        case EKeyCode::PageDown: return KEY_PAGE_DOWN;
        case EKeyCode::Home: return KEY_HOME;
        case EKeyCode::End: return KEY_END;
        case EKeyCode::CapsLock: return KEY_CAPS_LOCK;
        case EKeyCode::ScrollLock: return KEY_SCROLL_LOCK;
        case EKeyCode::NumLock: return KEY_NUM_LOCK;
        case EKeyCode::PrintScreen: return KEY_PRINT_SCREEN;
        case EKeyCode::PauseKey: return KEY_PAUSE;
        case EKeyCode::LeftShift: return KEY_LEFT_SHIFT;
        case EKeyCode::LeftControl: return KEY_LEFT_CONTROL;
        case EKeyCode::LeftAlt: return KEY_LEFT_ALT;
        case EKeyCode::LeftSuper: return KEY_LEFT_SUPER;
        case EKeyCode::RightShift: return KEY_RIGHT_SHIFT;
        case EKeyCode::RightControl: return KEY_RIGHT_CONTROL;
        case EKeyCode::RightAlt: return KEY_RIGHT_ALT;
        case EKeyCode::RightSuper: return KEY_RIGHT_SUPER;
        case EKeyCode::Numpad0: return KEY_KP_0;
        case EKeyCode::Numpad1: return KEY_KP_1;
        case EKeyCode::Numpad2: return KEY_KP_2;
        case EKeyCode::Numpad3: return KEY_KP_3;
        case EKeyCode::Numpad4: return KEY_KP_4;
        case EKeyCode::Numpad5: return KEY_KP_5;
        case EKeyCode::Numpad6: return KEY_KP_6;
        case EKeyCode::Numpad7: return KEY_KP_7;
        case EKeyCode::Numpad8: return KEY_KP_8;
        case EKeyCode::Numpad9: return KEY_KP_9;
        case EKeyCode::NumpadDecimal: return KEY_KP_DECIMAL;
        case EKeyCode::NumpadDivide: return KEY_KP_DIVIDE;
        case EKeyCode::NumpadMultiply: return KEY_KP_MULTIPLY;
        case EKeyCode::NumpadSubtract: return KEY_KP_SUBTRACT;
        case EKeyCode::NumpadAdd: return KEY_KP_ADD;
        case EKeyCode::NumpadEnter: return KEY_KP_ENTER;
        case EKeyCode::NumpadEqual: return KEY_KP_EQUAL;
        case EKeyCode::Apostrophe: return KEY_APOSTROPHE;
        case EKeyCode::Comma: return KEY_COMMA;
        case EKeyCode::Hyphen: return KEY_MINUS;
        case EKeyCode::Plus: return KEY_EQUAL;
        case EKeyCode::Period: return KEY_PERIOD;
        case EKeyCode::ForwardSlash: return KEY_SLASH;
        case EKeyCode::Semicolon: return KEY_SEMICOLON;
        case EKeyCode::Equal: return KEY_EQUAL;
        case EKeyCode::LeftBracket: return KEY_LEFT_BRACKET;
        case EKeyCode::Backslash: return KEY_BACKSLASH;
        case EKeyCode::RightBracket: return KEY_RIGHT_BRACKET;
        case EKeyCode::Tilde: return KEY_GRAVE;
        default: return 0;
        }
    }

    bool isHardwareKeyPressed(EKeyCode key)
    {
        return IsKeyPressed(translateToRaylibKey(key));
    }

    bool isHardwareKeyDown(EKeyCode key)
    {
        return IsKeyDown(translateToRaylibKey(key));
    }
}