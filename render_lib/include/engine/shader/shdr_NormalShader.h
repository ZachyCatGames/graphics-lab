#pragma once
#include "IShader.h"
#include "../ObjectBase.h"

namespace eng {

class NormalShader : public IShader, public ObjectBase<NormalShader> {
public:
    virtual Vector3DF GetColor(const HitStruct& rec) override;
}; // class NormalShader

} // namespace eng
