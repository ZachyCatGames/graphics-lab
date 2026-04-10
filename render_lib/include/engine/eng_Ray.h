#pragma once
#include <engine/eng_Vector3D.h>

namespace eng {

class Ray {
public:
    constexpr Ray() noexcept : m_origin{}, m_direction{} {}

    constexpr Ray(const Vector3DF& orig, const Vector3DF& dir) noexcept :
        m_origin(orig),
        m_direction(dir),
        m_invDir(1.0 / dir) {}

    [[nodiscard]] constexpr const Vector3DF& origin() const noexcept { return m_origin; }
    [[nodiscard]] constexpr const Vector3DF& direction() const noexcept { return m_direction; }
    [[nodiscard]] constexpr const Vector3DF& inverse_direction() const noexcept { return m_invDir; }

    [[nodiscard]] constexpr Vector3DF at(float t) const noexcept {
        return m_origin + m_direction * t;
    }

private:
    Vector3DF m_origin;
    Vector3DF m_direction;
    Vector3DF m_invDir;
}; // class Ray

} // namespace eng
