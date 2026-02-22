#pragma once
#include <engine/eng_Vector3D.h>

namespace eng {

class Ray {
public:
    constexpr Ray() : m_origin{}, m_direction{} {}

    constexpr Ray(const Vector3DF& orig, const Vector3DF& dir) : m_origin(orig), m_direction(dir) {}

    [[nodiscard]] constexpr const Vector3DF& origin() const { return m_origin; }
    [[nodiscard]] constexpr const Vector3DF& direction() const { return m_direction; }

    [[nodiscard]] constexpr Vector3DF at(float t) const {
        return m_origin + m_direction * t;
    }

private:
    Vector3DF m_origin;
    Vector3DF m_direction;
}; // class Ray

} // namespace eng
