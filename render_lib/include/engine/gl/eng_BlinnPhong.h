#pragma once
#include <engine/eng_IShader.h>
#include <engine/GLSL.h>
#include <engine/eng_Vector3D.h>
#include <engine/shader/shdr_PointLight.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace eng::gl {

class BlinnPhong : public IShader {
public:
    BlinnPhong();

    void Activate();

    void Deactivate() { m_shaderObject.deactivate(); }

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

    BlinnPhong& AssignDiffuseComponent(const Vector4DF& diffuseComponent) {
        m_diffuseComponent = diffuseComponent;
        return *this;
    }

    BlinnPhong& AssignSpecularComponent(const Vector4DF& specularComponent) {
        m_specularComponent = specularComponent;
        return *this;
    }

    BlinnPhong& AssignPhongExponent(float phongExponent) {
        m_phongExponent = phongExponent;
        return *this;
    }
private:
    sivelab::GLSLObject m_shaderObject;
    GLuint m_projectionMatrixId, m_viewMatrixId, m_modelMatrixId, m_normalMatrixId;
    GLuint m_lightPosWorldId, m_eyePosWorldId;
    GLuint m_diffuseComponentId, m_specularComponentId, m_phongExponentId;

public:
    glm::mat4 m_projectionMatrix, m_viewMatrix, m_modelMatrix, m_normalMatrix;
    Vector4DF m_lightPosition;
    Vector4DF m_eyePosition, m_diffuseComponent, m_specularComponent;
    float m_phongExponent;
}; // class BlinnPhong

} // namespace eng::gl
