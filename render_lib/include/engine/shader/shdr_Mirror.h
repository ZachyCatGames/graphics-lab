#pragma once
#include <engine/eng_IShader.h>
#include <engine/eng_ObjectBase.h>

namespace eng::shdr {

class Mirror : public IShader, public ObjectBase<Mirror> {
public:
    virtual Vector3DF GetColor(Scene* p_scene, const HitStruct& rec) override;
}; // class Mirror

} // namespace eng::shdr
