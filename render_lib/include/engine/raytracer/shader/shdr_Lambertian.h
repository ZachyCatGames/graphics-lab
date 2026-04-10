#pragma once
#include <engine/raytracer/eng_IShader.h>
#include <engine/eng_ObjectBase.h>
#include <ranges>

#include <engine/shader/shdr_PointLight.h>

namespace eng::rt::shdr {

class Lambertian : public IShader, public ObjectBase<Lambertian> {
public:
    template<std::ranges::input_range R>
    constexpr Lambertian(Handle<IShader> base_shader, R&& lights)
        : m_base(base_shader),
          m_lights(std::from_range, std::forward<R>(lights)) {}

    virtual Vector3DF GetColor(Scene* p_scene, int depth, const HitStruct& rec) override;
public:
    Handle<IShader> m_base;
    std::vector<eng::shdr::PointLight> m_lights;
}; // class Lambertian

} // namespace eng::shdr
