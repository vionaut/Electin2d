#include "el/core/containers/hash_map.hpp"
#include "el/input/input_manager.hpp"

using namespace el;

namespace el
{
    struct Impl
    {
        ElHashMap<EInputAction, EKeyCode> m_keyBindings;
    };

    ElInputManager::ElInputManager()
    {
        m_impl = new Impl;

        bindActionKey(EInputAction::Up, EKeyCode::W);
        bindActionKey(EInputAction::Right, EKeyCode::D);
        bindActionKey(EInputAction::Down, EKeyCode::S);
        bindActionKey(EInputAction::Left, EKeyCode::A);
        bindActionKey(EInputAction::Jump, EKeyCode::Space);
        bindActionKey(EInputAction::Quit, EKeyCode::Esc);
    }

    ElInputManager::~ElInputManager()
    {
        delete m_impl;
    }

    void ElInputManager::bindActionKey(EInputAction action, EKeyCode key)
    {
        m_impl->m_keyBindings[action] = key;
    }

    bool ElInputManager::isActionHeld(EInputAction action) const
    {
        auto it = m_impl->m_keyBindings.find(action);
        if (it != m_impl->m_keyBindings.end())
        {
            return isHardwareKeyDown(it->second);
        }
        return false;
    }

    bool ElInputManager::isActionPressed(EInputAction action) const
    {
        auto it = m_impl->m_keyBindings.find(action);
        if (it != m_impl->m_keyBindings.end())
        {
            return isHardwareKeyPressed(it->second);
        }
        return false;
    }
}
