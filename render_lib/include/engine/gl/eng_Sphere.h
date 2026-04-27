#pragma once
#include <engine/gl/eng_Mesh.h>

namespace eng::gl {

class Sphere : public Mesh {
public:
    Sphere(float radius, const Vector3DF& position, Handle<IShader> shader);

    [[nodiscard]] float GetRadius() const noexcept { return m_radius; }
private:
    float m_radius;
};

} // namespace eng
