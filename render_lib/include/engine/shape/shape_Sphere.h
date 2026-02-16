#pragma once
#include <engine/eng_ObjectBase.h>
#include <engine/shape/shape_IShape.h>

namespace eng {

class Sphere : public IShape, public eng::ObjectBase<Sphere> {
public:
    constexpr Sphere(const Vector3DF& pos, float radius) : m_position(pos), m_radius(radius) {}

    virtual bool Intersect(const ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const override;
private:
    Vector3DF m_position;
    float m_radius;
}; // class Sphere

} // namespace eng
