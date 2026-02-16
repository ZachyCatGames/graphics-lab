#pragma once
#include <engine/eng_IShader.h>
#include <engine/eng_ObjectBase.h>

namespace eng::shdr {

class FlatColor : public IShader, ObjectBase<FlatColor> {
public:
    constexpr FlatColor(const Vector3DF& color) : m_color(color) {}

    virtual Vector3DF GetColor(const HitStruct&) override { return m_color; }
public:
    Vector3DF m_color;
}; // class FlatColor

} // namespace eng::shdr
