#pragma once
#include <engine/eng_HandleGraphicsArgs.h>
#include <engine/eng_PerspectiveCamera.h>
#include <engine/eng_ThreadedRenderer.h>
#include <engine/eng_Scene.h>
#include <engine/demo/demo_DrawFramebuffer.h>
#include <engine/framebuffer/fb_Framebuffer.h>

namespace eng::demo {

class SceneRenderHelper {
public:
    SceneRenderHelper(int argc, char** argv);

    template<typename Self>
    auto GetScene(this Self&& self) noexcept { return &self.m_scene; }

    template<typename Self>
    auto GetFramebuffer(this Self&& self) noexcept { return &self.m_fb; }

    template<typename Self>
    auto GetCamera(this Self&& self) noexcept { return &self.m_camera; }

    auto GetArgs() const noexcept { return &m_args; }

    void RenderScene();
    void DrawFramebuffer() { demo::DrawFramebuffer(m_fb); }
private:
    ThreadedRenderer m_renderer;
    Scene m_scene;
    GraphicsArgs m_args;
    fb::Framebuffer m_fb;
    int m_img_width, m_img_height;
}; // class SceneRenderHelper

} // namespace eng::demo
