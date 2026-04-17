#include <engine/eng_RenderComponent.h>

namespace eng {

RenderComponent::RenderComponent(Handle<Object> parent) :
    Component(parent)
{
    m_RO.mesh = GetParent()->GetComponent<Mesh>();
}

void RenderComponent::Update() {

}

} // namespace eng
