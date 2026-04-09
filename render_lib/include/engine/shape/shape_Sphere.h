#pragma once
#include <engine/eng_ShapeBase.h>
#include <engine/eng_ObjectBase.h>

namespace eng::shape {

class Sphere : public ShapeBase, public eng::ObjectBase<Sphere> {
public:
    constexpr Sphere(const Vector3DF& pos, float radius, Handle<IShader> shader) :
        m_position(pos),
        m_radius(radius),
        ShapeBase(shader) {}

    virtual bool Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const override;

    virtual Vector3DF GetPosition() const override;
    virtual Bounds GetBounds() const override;
private:
    Vector3DF m_position;
    float m_radius;
}; // class Sphere

} // namespace eng::shape
