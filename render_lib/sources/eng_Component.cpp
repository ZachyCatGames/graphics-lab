#include <engine/eng_Component.h>

namespace eng {

void Component::EarlyUpdateImpl() {
    /* Only update if we and our parent are active. */
    if (m_Active && m_Parent->IsActive())
        this->EarlyUpdate();
}

void Component::UpdateImpl() {
    /* Only update if we and our parent are active. */
    if (m_Active && m_Parent->IsActive())
        this->Update();
}

} // namespace eng
