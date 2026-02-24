#pragma once
#include <engine/eng_IShader.h>
#include <engine/eng_ObjectBase.h>

#include <engine/shader/detail/shdr_LambertianImpl.h>

namespace eng::shdr {

class BlinnPhong : public IShader, public ObjectBase<BlinnPhong> {
public:
    constexpr BlinnPhong(Handle<IShader> base_shader, Vector3DF light_position, Vector3DF light_color, float exp) :
        m_lambertian(base_shader, light_position, light_color), m_exp(exp) {}

    virtual Vector3DF GetColor(Scene* p_scene, const HitStruct& rec) override;
private:
    detail::LambertianImpl m_lambertian;
    float m_exp;
}; // class BlinnPhong

} // namespace eng::shdr
