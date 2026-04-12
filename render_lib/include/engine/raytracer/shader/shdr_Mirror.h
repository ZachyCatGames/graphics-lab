#pragma once
#include <engine/raytracer/eng_IShader.h>
#include <engine/eng_ObjectBase.h>

namespace eng::rt::shdr {

class Mirror : public IShader, public ObjectBase<Mirror> {
public:
    virtual Vector3DF GetColor(RayCaster* pRc, int depth, const HitStruct& rec) override;
}; // class Mirror

} // namespace eng::shdr
