#pragma once
#include <engine/eng_HitStruct.h>
#include <engine/eng_Ray.h>

#include <engine/eng_IShader.h>
#include <engine/eng_ObjectManager.h>
#include <engine/eng_Scene.h>

namespace eng::shdr::detail {

class LambertianImpl {
public:
    constexpr LambertianImpl(Handle<IShader> base_shader, Vector3DF light_position, Vector3DF light_intensity) : 
        m_light_position(light_position), m_light_intensity(light_intensity) {}

    [[nodiscard]] Vector3DF GetColor(Scene* p_scene, const HitStruct& rec);

    constexpr auto GetDirection(Vector3DF dst_pos) {
        return (m_light_position - dst_pos).normalize();
    }
public:
    Handle<IShader> m_base;
    Vector3DF m_light_position;
    Vector3DF m_light_intensity;
}; // class LambertianImpl

} // namespace eng::shdr::detail
