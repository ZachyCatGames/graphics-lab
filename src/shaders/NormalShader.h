#pragma once
#include "IShader.h"

namespace eng {

class NormalShader : public IShader {
public:
    virtual Vector3DF GetColor(const HitStruct& rec) override;
}; // class NormalShader

} // namespace eng
