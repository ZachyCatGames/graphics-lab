#pragma once
#include <engine/raytracer/eng_HitStruct.h>
#include <engine/raytracer/eng_MeshType.h>
#include <engine/eng_Interval.h>
#include <engine/eng_ObjectManager.h>
#include <engine/eng_Ray.h>

#include <engine/eng_Material.h>
#include <engine/eng_Translation.h>
#include <engine/eng_Shape.h>
#include <engine/raytracer/eng_IShader.h>

namespace eng::rt {

class IShader;

class Shape {
public:
    virtual bool Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const = 0;

    //virtual Handle<IShader> GetShader() const = 0;
    //virtual Handle<Material> GetMaterial() const = 0;
    //virtual Handle<Translation> GetTranslation() const = 0;

    //virtual Bounds GetBounds() const = 0;
}; // class Mesh

class ShapeHolderBase : public Shape {
public:
    Handle<IShader> m_Shader;
    Handle<Material> m_Mat;
    Handle<Translation> m_Trans;
};

template<typename T>
class ShapeHolder : public ShapeHolderBase {
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

} // namespace eng::shape
