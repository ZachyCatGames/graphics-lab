#pragma once
#include <engine/eng_ObjectBase.h>
#include <engine/shader/shdr_IShader.h>

namespace eng {

class NormalShader : public IShader, public ObjectBase<NormalShader> {
public:
    virtual Vector3DF GetColor(const HitStruct& rec) override;
}; // class NormalShader

} // namespace eng
