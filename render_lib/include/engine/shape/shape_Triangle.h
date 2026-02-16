#pragma once
#include <engine/eng_ObjectBase.h>
#include <engine/shape/shape_IShape.h>

namespace eng {

class Triangle : public IShape, public ObjectBase<Triangle> {
public:
    constexpr Triangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c) :
        m_a(a), m_b(b), m_c(c) {}

    virtual bool Intersect(const ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const override;
private:
    Vector3DF m_a, m_b, m_c;
}; // class Triangle

} // namespace eng
