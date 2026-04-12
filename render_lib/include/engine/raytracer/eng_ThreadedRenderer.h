#pragma once
#include <engine/raytracer/eng_RendererBase.h>

namespace eng::rt {

class ThreadedRenderer : public RendererBase {
public:
    ThreadedRenderer() : m_thread_count(1) {}

    ThreadedRenderer(const std::shared_ptr<eng::Scene>& scene, int rpp, int randpix, int maxDepth, size_t thread_count) :
        RendererBase(scene, rpp, randpix, maxDepth),
        m_pScene(scene),
        m_thread_count(thread_count) {}

    void Initialize(const std::shared_ptr<eng::Scene>& scene, int rpp, int randpix, int maxDepth, size_t thread_count) {
        RendererBase::Initialize(scene, rpp, randpix, maxDepth);
        m_pScene = scene;
        m_thread_count = thread_count;
    }

    virtual void PreRender() override;

    virtual void Render(std::string_view cameraName, fb::Framebuffer* p_fb) override;
private:
    std::shared_ptr<Scene> m_pScene;
    size_t m_thread_count;
}; // class ThreadedRenderer

} // namespace eng
