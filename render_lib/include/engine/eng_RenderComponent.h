#pragma once
#include <engine/eng_Component.h>
#include <engine/eng_IShader.h>
#include <engine/eng_ObjectManager.h>
#include <engine/eng_Material.h>
#include <engine/eng_Mesh.h>
#include <engine/eng_Translation.h>

namespace eng {

class RenderComponent : public Component {
public:
    RenderComponent(Handle<Object> parent);

    virtual void Update() override;

    Handle<Mesh> m_Mesh;
    Handle<IShader> m_Shader;
    Handle<Material> m_Material;
    Handle<Translation> m_Pos;
    // TODO: Mesh
    // TODO: Material
    // TODO: Shader
}; // class RenderComponent

} // namespace eng
