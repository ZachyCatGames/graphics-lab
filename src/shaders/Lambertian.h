#pragma once
#include "IShader.h"
#include "detail/LambertianImpl.h"
#include "../ObjectBase.h"

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
