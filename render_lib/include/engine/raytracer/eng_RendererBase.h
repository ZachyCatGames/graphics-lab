#pragma once
#include <engine/eng_Engine.h>
#include <engine/eng_IRenderer.h>
#include <engine/raytracer/eng_RayCaster.h>
#include <engine/framebuffer/fb_Framebuffer.h>
#include <engine/raytracer/eng_ICamera.h>

#include <concepts>
#include <type_traits>
#include <unordered_map>

namespace eng::rt {

class RendererBase : public IRenderer {
public:
    RendererBase() = default;

    RendererBase(const std::shared_ptr<eng::Scene>& scene, int rpp, int randpix, int maxDepth) :
        m_caster(scene, maxDepth),
        m_raysPerPixel(rpp),
        m_randomSamples(randpix) { /* ... */ }

    virtual ~RendererBase() = default;

    int Initialize(const std::shared_ptr<eng::Scene>& scene, int rpp, int randpix, int maxDepth) {
        m_raysPerPixel = rpp;
        m_randomSamples = randpix;
        return m_caster.Initialize(scene, maxDepth);
    }

    void Finalize() { m_caster.Finalize(); }

    [[nodiscard]] bool IsInitialized() const noexcept { return m_caster.IsInitialized(); }

    RayCaster* GetCaster() { return &m_caster; }
protected:
    Vector3DF GetPixelColor(Handle<ICamera> p_cam, int x, int y);
protected:
    RayCaster m_caster;

    size_t m_raysPerPixel;
    bool m_randomSamples;
}; // class RendererBase

} // namespace eng
