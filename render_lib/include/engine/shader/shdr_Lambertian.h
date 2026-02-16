#pragma once
#include <engine/eng_ObjectBase.h>
#include <engine/shader/shdr_IShader.h>

#include <engine/shader/detail/shdr_LambertianImpl.h>

namespace eng {


//public ObjectBase<LambertianShader>
class LambertianShader : public IShader, public ObjectBase<LambertianShader>, private detail::LambertianShaderImpl {
public:
    constexpr LambertianShader(ray point_light) : detail::LambertianShaderImpl(point_light) {}

    virtual Vector3DF GetColor(const HitStruct& rec) override {
        return detail::LambertianShaderImpl::GetColor(rec);
    }
}; // class LambertianShader

} // namespace eng
