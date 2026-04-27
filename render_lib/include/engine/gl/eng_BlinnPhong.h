#pragma once
#include <engine/eng_IShader.h>
#include <engine/GLSL.h>
#include <engine/eng_ObjectBase.h>
#include <engine/eng_Material.h>
#include <engine/eng_Vector3D.h>
#include <engine/shader/shdr_PointLight.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace eng::gl {

class BlinnPhong : public IShader, public ObjectBase<BlinnPhong> {
public:
    BlinnPhong(const Material& material);

    void Activate();

    void Deactivate() { m_shaderObject.deactivate(); }

    virtual const Material* GetMaterial() const override;

    BlinnPhong& AssignProjectionMatrix(const glm::mat4& projectionMatrix) {
        m_projectionMatrix = projectionMatrix;
        return *this;
    }

    BlinnPhong& AssignViewMatrix(const glm::mat4& viewMatrix) {
        m_viewMatrix = viewMatrix;
        return *this;
    }

    BlinnPhong& AssignModelMatrix(const glm::mat4& modelMatrix) {
        m_modelMatrix = modelMatrix;
        return *this;
    }

    BlinnPhong& AssignNormalMatrix(const glm::mat4& normalMatrix) {
        m_normalMatrix = normalMatrix;
        return *this;
    }

    BlinnPhong& AssignLightPosition(const Vector4DF& lightPosition) {
        m_lightPosition = lightPosition;
        return *this;
    }

    BlinnPhong& AssignEyePosition(const Vector4DF& eyePosition) {
        m_eyePosition = eyePosition;
        return *this;
    }
public:
    sivelab::GLSLObject* GetShaderObject() { return &m_shaderObject; }
    void SetTextureId(GLuint texId) { m_textureSamplerId = texId; }
private:
    sivelab::GLSLObject m_shaderObject;
    GLuint m_projectionMatrixId, m_viewMatrixId, m_modelMatrixId, m_normalMatrixId;
    GLuint m_lightPosWorldId, m_eyePosWorldId;
    GLuint m_diffuseComponentId, m_specularComponentId, m_phongExponentId;
    GLuint m_textureSamplerId;
public:
    Material m_Material;
    glm::mat4 m_projectionMatrix, m_viewMatrix, m_modelMatrix, m_normalMatrix;
    Vector4DF m_lightPosition;
    Vector4DF m_eyePosition;
    GLuint m_textureSampler;
}; // class BlinnPhong

} // namespace eng::gl
