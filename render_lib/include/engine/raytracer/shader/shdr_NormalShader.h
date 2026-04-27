#pragma once
#include <engine/raytracer/eng_IShader.h>

namespace eng::rt::shdr {

class Normal : public IShader {
public:
    [[nodiscard]] Vector3DF GetColor(RayCaster* pRc, int depth, const HitStruct& rec) override;

    [[nodiscard]] Material GetMaterial() const override;
}; // class Normal

} // namespace eng::shdr
