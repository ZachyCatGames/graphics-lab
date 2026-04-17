#pragma once
#include <engine/eng_ObjectPool.h>
#include <cassert>

namespace eng {

class Object;

class Component {
public:
    constexpr Component(Handle<Object> parent, bool defaultActive = true) :
        m_Parent(parent),
        m_Active(defaultActive)
    {
        assert(parent.IsValid());
    }

    constexpr auto GetParent() const noexcept { return m_Parent; }

    virtual void Initialize() {}
    virtual void Destroy() {}

    virtual void EarlyUpdate() {}
    virtual void Update() {}

    constexpr bool IsActive() const noexcept { return m_Active; }
    constexpr void SetActive(bool active) noexcept { m_Active = active; }

    void EarlyUpdateImpl();
    void UpdateImpl();
private:
    Handle<Object> m_Parent;
    bool m_Active;
}; // class Component

} // namespace eng
