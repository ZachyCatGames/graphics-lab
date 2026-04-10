#pragma once
#include <engine/raytracer/eng_IShader.h>
#include <engine/eng_ObjectBase.h>
#include <engine/eng_ObjectManager.h>

#include <engine/shader/shdr_PointLight.h>

namespace eng::rt::shdr {

class BlinnPhong : public IShader, public ObjectBase<BlinnPhong> {
public:
    template<typename R>
    constexpr BlinnPhong(Handle<IShader> base_shader, R&& lights, float exp)
        : m_base(base_shader),
          m_lights(std::from_range, std::forward<R>(lights)),
          m_exp(exp) {}

    virtual Vector3DF GetColor(Scene* p_scene, int depth, const HitStruct& rec) override;
private:
    Handle<IShader> m_base;
    std::vector<eng::shdr::PointLight> m_lights;
    float m_exp;
}; // class BlinnPhong

} // namespace eng::shdr
