#pragma once
#include <engine/eng_IRenderer.h>
#include <engine/eng_Scene.h>
#include <memory>

namespace eng::gl {

class Renderer : public IRenderer {
public:
    Renderer(std::shared_ptr<Scene> scene, bool clearFb = true) :
    m_Scene(scene),
    m_ClearFrambuffer(clearFb) {}

    virtual void PreRender() override;

    virtual void Render(std::string_view cameraName, Handle<RenderBuffer> fb) override;
private:
    std::shared_ptr<Scene> m_Scene;
    bool m_ClearFrambuffer;
}; // class Renderer

} // namespace eng::gl
