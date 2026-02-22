#pragma once
#include <engine/eng_IShader.h>
#include <engine/eng_ObjectBase.h>

#include <engine/shader/detail/shdr_LambertianImpl.h>

namespace eng::shdr {

class Lambertian : public IShader, public ObjectBase<Lambertian>, private detail::LambertianImpl {
public:
    constexpr Lambertian(Ray point_light) : detail::LambertianImpl(point_light) {}

    virtual Vector3DF GetColor(const HitStruct& rec) override {
        return detail::LambertianImpl::GetColor(rec);
    }
}; // class Lambertian

} // namespace eng::shdr
