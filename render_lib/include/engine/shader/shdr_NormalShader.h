#pragma once
#include <engine/eng_IShader.h>
#include <engine/eng_ObjectBase.h>

namespace eng::shdr {

class Normal : public IShader, public ObjectBase<Normal> {
public:
    virtual Vector3DF GetColor(const HitStruct& rec) override;
}; // class Normal

} // namespace eng::shdr
