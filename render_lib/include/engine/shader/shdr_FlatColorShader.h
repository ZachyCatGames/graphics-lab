#pragma once
#include <engine/eng_ObjectBase.h>
#include <engine/shader/shdr_IShader.h>

namespace eng {

class FlatColorShader : public IShader, ObjectBase<FlatColorShader> {
public:
    constexpr FlatColorShader(const Vector3DF& color) : m_color(color) {}

    virtual Vector3DF GetColor(const HitStruct&) override { return m_color; }
public:
    Vector3DF m_color;
}; // class FlatColorShader

} // namespace eng
