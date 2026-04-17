#pragma once
#include <engine/raytracer/eng_Shape.h>
#include <engine/eng_Vector3D.h>
#include <engine/eng_Bounds.h>

namespace eng::rt::shape {

class Sphere : public Shape {
public:
    constexpr Sphere(const Vector3DF& pos, float radius) :
        m_position(pos),
        m_radius(radius) {}

    virtual bool Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const override;

    Bounds GetBounds() const;
private:
    Vector3DF m_position;
    float m_radius;
}; // class Sphere

} // namespace eng::shape
