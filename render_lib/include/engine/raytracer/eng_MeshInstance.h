#pragma once
#include <engine/raytracer/eng_Mesh.h>
#include <engine/eng_Material.h>

namespace eng::rt {

template<typename T>
class MeshInstance : public Mesh {
public:
    constexpr MeshInstance(Handle<T> impl) : m_Impl(impl) {}

    virtual bool Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const override {
        return m_Impl->Intersect(r, t_range, p_hit_info_out);
    }

    virtual Handle<IShader> GetShader() const = 0;

    virtual Bounds GetBounds() const = 0;
private:
    Handle<T> m_Impl;
    Handle<Material> m_Material;
}; // class MeshResource

} // namespace eng::rt
