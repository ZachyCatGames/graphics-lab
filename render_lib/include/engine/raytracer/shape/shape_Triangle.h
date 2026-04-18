#pragma once
#include <engine/raytracer/eng_ShapeBase.h>
#include <engine/eng_ObjectBase.h>

namespace eng::rt::shape {

class Triangle : public ShapeBase, public ObjectBase<Triangle> {
public:
    constexpr Triangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, Handle<IShader> shader) :
        ShapeBase(shader),
        m_a(a), m_b(b), m_c(c) {}

    virtual bool Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const override;

    virtual Vector3DF GetPosition() const override;
    virtual Bounds GetBounds() const override;
private:
    Bounds CalculateBounds() const noexcept;
private:
    Vector3DF m_a, m_b, m_c;
    Bounds m_Bounds;
}; // class Triangle

} // namespace eng::shape
