#pragma once
#include <engine/eng_IShader.h>
#include <engine/eng_ObjectBase.h>

#include <engine/shader/detail/shdr_LambertianImpl.h>

namespace eng::shdr {

class Lambertian : public IShader, public ObjectBase<Lambertian> {
public:
    constexpr Lambertian(Handle<IShader> base_shader, Vector3DF light_position, Vector3DF light_intensity) :
        m_base(base_shader, light_position, light_intensity) {}

    virtual Vector3DF GetColor(Scene* p_scene, int depth, const HitStruct& rec) override {
        return m_base.GetColor(p_scene, depth, rec);
    }
public:
    detail::LambertianImpl m_base;
}; // class Lambertian

} // namespace eng::shdr
