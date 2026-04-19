#include <engine/gl/eng_BlinnPhong.h>

namespace eng::gl {

namespace {

static void AssignMat4(GLuint id, const glm::mat4& matrix) {
    glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(matrix));
}

static void AssignVec4(GLuint id, const Vector4DF& vector) {
    glUniform4fv(id, 1, vector.get_ptr());
}

} // namespace

BlinnPhong::BlinnPhong() {
    /* Setup shader object w/ the blinn phong vertex and fragment shader. */
    m_shaderObject.addShader("vertexShader_BlinnPhong.glsl", sivelab::GLSLObject::VERTEX_SHADER);
    m_shaderObject.addShader("fragmentShader_BlinnPhong.glsl", sivelab::GLSLObject::FRAGMENT_SHADER);
    m_shaderObject.createProgram();

    /* Setup uniforms for transform matrices. */
    m_projectionMatrixId = m_shaderObject.createUniform("projMatrix");
    m_viewMatrixId       = m_shaderObject.createUniform("viewMatrix");
    m_modelMatrixId      = m_shaderObject.createUniform("modelMatrix");
    m_normalMatrixId     = m_shaderObject.createUniform("normalMatrix");

    /* Setup position uniforms. */
    m_lightPosWorldId = m_shaderObject.createUniform("lightPosWorld");
    m_eyePosWorldId   = m_shaderObject.createUniform("eyePosWorld");

    /* Setup lighting parameter uniforms. */
    m_diffuseComponentId  = m_shaderObject.createUniform("diffuseComponent");
    m_specularComponentId = m_shaderObject.createUniform("specularComponent");
    m_phongExponentId     = m_shaderObject.createUniform("phongExponent");
}

void BlinnPhong::Activate() {
    m_shaderObject.activate();

    AssignMat4(m_projectionMatrixId, m_projectionMatrix);
    AssignMat4(m_viewMatrixId, m_viewMatrix);
    AssignMat4(m_modelMatrixId, m_modelMatrix);
    AssignMat4(m_normalMatrixId, m_normalMatrix);

    AssignVec4(m_lightPosWorldId, m_lightPosition);
    AssignVec4(m_eyePosWorldId, m_eyePosition);
    AssignVec4(m_diffuseComponentId, m_diffuseComponent);
    AssignVec4(m_specularComponentId, m_specularComponent);

    glUniform1f(m_phongExponentId, m_phongExponent);
}

} // namespace eng::gl
