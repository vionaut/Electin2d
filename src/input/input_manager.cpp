#include "vortex/core/containers/hash_map.hpp"
#include "vortex/input/input_manager.hpp"

using namespace vortex;

namespace vortex::input
{
    struct Impl
    {
        containers::VxHashMap<EInputAction, EKeyCode> m_keyBindings;
    };

    VxInputManager::VxInputManager()
    {
        m_impl = new Impl;

        bindActionKey(EInputAction::Up, EKeyCode::W);
        bindActionKey(EInputAction::Right, EKeyCode::D);
        bindActionKey(EInputAction::Down, EKeyCode::S);
        bindActionKey(EInputAction::Left, EKeyCode::A);
        bindActionKey(EInputAction::Jump, EKeyCode::Space);
        bindActionKey(EInputAction::Quit, EKeyCode::Esc);
    }

    VxInputManager::~VxInputManager()
    {
        delete m_impl;
    }

    void VxInputManager::bindActionKey(EInputAction action, EKeyCode key)
    {
        m_impl->m_keyBindings[action] = key;
    }

    bool VxInputManager::isActionHeld(EInputAction action) const
    {
        auto it = m_impl->m_keyBindings.find(action);
        if (it != m_impl->m_keyBindings.end())
        {
            return isHardwareKeyDown(it->second);
        }
        return false;
    }

    bool VxInputManager::isActionPressed(EInputAction action) const
    {
        auto it = m_impl->m_keyBindings.find(action);
        if (it != m_impl->m_keyBindings.end())
        {
            return isHardwareKeyPressed(it->second);
        }
        return false;
    }
}
