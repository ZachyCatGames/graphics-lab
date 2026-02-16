#pragma once
#include "vec.h"

namespace eng {

class ray {
public:
    constexpr ray() : m_origin{}, m_direction{} {}

    constexpr ray(const Vector3DF& orig, const Vector3DF& dir) : m_origin(orig), m_direction(dir) {}

    constexpr const Vector3DF& origin() const { return m_origin; }
    constexpr const Vector3DF& direction() const { return m_direction; }

    constexpr Vector3DF at(float t) const {
        return m_origin + m_direction * t;
    }

private:
    Vector3DF m_origin;
    Vector3DF m_direction;
}; // class ray

} // namespace eng
