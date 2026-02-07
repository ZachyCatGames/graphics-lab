#pragma once
#include "IShape.h"

namespace eng {

class Sphere : public IShape {
public:
    constexpr Sphere(const Vector3DF& pos, float radius) : m_position(pos), m_radius(radius) {}

    virtual bool Intersect(const ray& r, float tmin, float* p_tmax, HitStruct* p_hit_info_out) override;
private:
    Vector3DF m_position;
    float m_radius;
}; // class Sphere

} // namespace eng
