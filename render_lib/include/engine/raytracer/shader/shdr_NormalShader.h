#pragma once
#include <engine/raytracer/eng_IShader.h>
#include <engine/eng_ObjectBase.h>

namespace eng::rt::shdr {

class Normal : public IShader, public ObjectBase<Normal> {
public:
    virtual Vector3DF GetColor(RayCaster* pRc, int depth, const HitStruct& rec) override;
}; // class Normal

} // namespace eng::shdr
