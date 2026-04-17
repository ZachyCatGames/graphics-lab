#pragma once
#include <engine/eng_Scene.h>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace eng {

class Component;

class Object {
public:
    constexpr Object(Scene* pParent, bool defaultActive = true) : m_pParent(pParent) {}

    constexpr auto GetParent() const noexcept { return m_pParent; }

    constexpr std::string_view GetName() const noexcept { return m_Name; }

    constexpr bool IsActive() const noexcept { return m_Active; }
    constexpr void SetActive(bool active) noexcept { m_Active = false; }

    void EarlyUpdate();
    void Update();

    template<typename ComponentType, typename... Args>
    Handle<ComponentType> AddComponent(Args&&... args) {
        static constexpr auto id = typeid(ComponentType);

        /* Do we already have one of these? */
        if (m_ComponentMap.find(id) != m_ComponentManagerMap.end())
            return nullptr;

        /* Allocate the new component. */
        auto comp = m_pParent->CreateComponent<ComponentType>(std::forward<Args>(args)...);

        /* Add it to our list and map. */
        m_ComponentMap[id] = comp;
        m_Components.push_back(comp);

        return comp;
    }
private:
    Scene* m_pParent;
    std::unordered_map<std::type_info, Handle<Component>> m_ComponentMap;
    std::vector<Handle<Component>> m_Components;
    std::string m_Name;
    bool m_Active;
}; // class Object

} // namespace eng
