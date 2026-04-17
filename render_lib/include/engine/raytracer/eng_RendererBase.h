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

class RenderShapeBase : public Shape {
public:
    Handle<IShader> m_Shader;
    Handle<Material> m_Mat;
    Handle<Translation> m_Trans;
};

template<typename T>
class RenderShape : public RenderShapeBase {
public:
    virtual bool Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const {
        Ray newRay(r.origin() + m_Trans->pos, r.direction());

        if (m_Impl->Intersect(newRay, t_range, p_hit_info_out)) {
            p_hit_info_out->shader = m_Shader.Get();
            p_hit_info_out->material = m_Mat.Get();
            return true;
        }

        return false;
    }
private:
    Handle<T> m_Impl;
};

class RendererBase : public Renderer {
public:
    RendererBase() = default;

    RendererBase(int rpp, int randpix, int maxDepth) :
        m_caster(&m_RenderObjects, maxDepth),
        m_raysPerPixel(rpp),
        m_randomSamples(randpix) { /* ... */ }

    virtual ~RendererBase() = default;

    int Initialize(int rpp, int randpix, int maxDepth) {
        m_raysPerPixel = rpp;
        m_randomSamples = randpix;
        m_caster.Initialize(&m_RenderObjects, maxDepth);
    }

    void Finalize() { m_caster.Finalize(); }

    [[nodiscard]] bool IsInitialized() const noexcept { return m_caster.IsInitialized(); }

    virtual void AddRenderObject(Handle<Shape> mesh, Handle<Material> mat, Handle<Translation> pos);

    virtual void ClearRenderObjects();

    virtual void ReserveRenderObjects(size_t num);

    RayCaster* GetCaster() { return &m_caster; }
protected:
    Vector3DF GetPixelColor(Handle<ICamera> p_cam, int x, int y);
protected:
    RayCaster m_caster;

    std::vector<RenderShape> m_RenderShapes;
    std::vector<RenderShape*> m_pRenderShapes;

    size_t m_raysPerPixel;
    bool m_randomSamples;
}; // class RendererBase

} // namespace eng
