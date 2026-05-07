#pragma once
#include <engine/raytracer/eng_IShader.h>

namespace eng::rt::shdr {

class FlatColor : public IShader {
public:
    constexpr FlatColor(const Vector3DF& color) : m_color(color) {}

    [[nodiscard]] Vector3DF GetColor(RayCaster* pRc, int depth, const HitStruct&) override;

    [[nodiscard]] Material GetMaterial() const override;
public:
    Vector3DF m_color;
}; // class FlatColor

} // namespace eng::shdr
