#include <engine/eng_Object.h>

namespace eng {

void Object::EarlyUpdate() {
    for (auto& component : m_Components) {
        component->EarlyUpdate();
    }
}

void Object::Update() {
    for (auto& component : m_Components) {
        component->Update();
    }
}

} // namespace eng
