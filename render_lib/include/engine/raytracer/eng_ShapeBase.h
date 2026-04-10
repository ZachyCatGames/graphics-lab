#pragma once
#include <engine/raytracer/eng_IShape.h>

namespace eng::rt {

class ShapeBase : public IShape {
public:
    constexpr ShapeBase(Handle<IShader> shader) : m_shader(shader) {}

    virtual Handle<IShader> GetShader() const override { return m_shader; }
protected:
    Handle<IShader> m_shader;
}; // class ShapeBase

} // namespace eng::rt
