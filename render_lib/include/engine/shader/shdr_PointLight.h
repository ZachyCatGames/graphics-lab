#pragma once
#include <engine/eng_ObjectBase.h>
#include <engine/eng_Vector3D.h>

namespace eng::shdr {

struct PointLight : public ObjectBase<PointLight> {
    constexpr PointLight(const Vector3DF& pos, const Vector3DF& intense)
        : position(pos),
          intensity(intense) {}

    constexpr auto GetDirection(const Vector3DF dst_pos) const {
        return (position - dst_pos).normalize();
    }

    Vector3DF position;
    Vector3DF intensity;
}; // struct PointLight

} // namespace eng::shdr
