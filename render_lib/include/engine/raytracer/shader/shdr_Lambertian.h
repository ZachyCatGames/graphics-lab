#pragma once
#include <engine/raytracer/eng_ShaderBase.h>

namespace eng::rt::shdr {

class Lambertian : public ShaderBase {
public:
    constexpr Lambertian(const Material& material)
        : ShaderBase(material) {}

    virtual Vector3DF GetColor(RayCaster* pRc, int depth, const HitStruct& rec) override;
}; // class Lambertian

} // namespace eng::shdr
