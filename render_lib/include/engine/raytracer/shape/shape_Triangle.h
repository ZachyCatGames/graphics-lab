#pragma once
#include <engine/raytracer/eng_Shape.h>

namespace eng::rt::shape {

class Triangle : public Shape {
public:
    constexpr Triangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c) :
        m_a(a), m_b(b), m_c(c) {}

    virtual bool Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const override;

    Vector3DF GetPosition() const;
    Bounds GetBounds() const;
private:
    Vector3DF m_a, m_b, m_c;
}; // class Triangle

} // namespace eng::shape
