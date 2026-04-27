#pragma once
#include <engine/raytracer/eng_IShader.h>

namespace eng::rt {

class ShaderBase : public IShader {
public:
    constexpr ShaderBase(const Material& material) : m_Material(material) {}

    virtual ~ShaderBase() = default;

    virtual const Material* GetMaterial() const override { return &m_Material; }
protected:
    Material m_Material;
}; // class ShaderBase

} // namespace eng::rt
