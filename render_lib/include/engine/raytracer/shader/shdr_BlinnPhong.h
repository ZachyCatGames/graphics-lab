#pragma once
#include <engine/raytracer/eng_ShaderBase.h>

namespace eng::rt::shdr {

class BlinnPhong : public ShaderBase {
public:
    constexpr BlinnPhong(const Material& material)
        : ShaderBase(material) {}

    virtual Vector3DF GetColor(RayCaster* pRc, int depth, const HitStruct& rec) override;
}; // class BlinnPhong

} // namespace eng::shdr
