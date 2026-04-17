#include <engine/eng_Renderer.h>

namespace eng {

Renderer::~Renderer() = default;

void Renderer::AddRenderObject(const RenderObject& ro) {
    m_RenderObjects.push_back(ro);
}

void Renderer::ClearRenderObjects() {
    m_RenderObjects.clear();
}

void Renderer::ReserveRenderObjects(size_t num) {
    m_RenderObjects.reserve();
}

} // namespace eng
