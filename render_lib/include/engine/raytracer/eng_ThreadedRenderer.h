#pragma once
#include <engine/raytracer/eng_RendererBase.h>

namespace eng::rt {

class ThreadedRenderer : public RendererBase {
public:
    ThreadedRenderer() : m_thread_count(1) {}

    ThreadedRenderer(Scene* p_scene, size_t default_img_width, size_t default_img_height, size_t samples_per_pixel, bool random_samples, size_t thread_count)
        : RendererBase(p_scene, default_img_width, default_img_height, samples_per_pixel, random_samples),
          m_thread_count(thread_count) {}

    void Initialize(Scene* p_scene, size_t default_img_width, size_t default_img_height, size_t samples_per_pixel, bool random_samples, size_t thread_count) {
        RendererBase::Initialize(p_scene, default_img_width, default_img_height, samples_per_pixel, random_samples);
        m_thread_count = thread_count;
    }

    virtual void Render(int camera_id, fb::Framebuffer* p_fb) override;
private:
    size_t m_thread_count;
}; // class ThreadedRenderer

} // namespace eng
