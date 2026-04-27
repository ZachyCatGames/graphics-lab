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

BlinnPhong::BlinnPhong(const Material& material) :
    m_texture(material.texture.StaticCast<Texture>()),
    m_ambientLight(material.ambientLight),
    m_diffuseComponent(material.diffuse),
    m_specularComponent(material.specular),
    m_phongExponent(material.shininess)
{
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

    m_textureSamplerId = m_shaderObject.createUniform("textureSampler");
}

void BlinnPhong::Activate() {
    /* Enable our texture if we have one. */
    if (m_texture)
        m_texture->Bind(0);

    /* Activate our shader object and copy uniforms over. */
    m_shaderObject.activate();

    AssignMat4(m_projectionMatrixId, m_projectionMatrix);
    AssignMat4(m_viewMatrixId, m_viewMatrix);
    AssignMat4(m_modelMatrixId, m_modelMatrix);
    AssignMat4(m_normalMatrixId, m_normalMatrix);

    AssignVec4(m_lightPosWorldId, m_lightPosition);
    AssignVec4(m_eyePosWorldId, m_eyePosition);
    AssignVec4(m_diffuseComponentId, Vector4DF(m_diffuseComponent, 1.0));
    AssignVec4(m_specularComponentId, Vector4DF(m_specularComponent, 1.0));

    glUniform1f(m_phongExponentId, m_phongExponent);

    glUniform1i(m_textureSamplerId, m_textureSampler);
}

void BlinnPhong::Deactivate() {
    /* Deactivate our shader object. */
    m_shaderObject.deactivate();

    /* Unbind our texture if we have one. */
    if (m_texture)
        m_texture->Unbind();
}

Material BlinnPhong::GetMaterial() const {
    return Material{
        .texture        = m_texture,
        .ambientLight   = m_ambientLight,
        .diffuse        = m_diffuseComponent,
        .specular       = m_specularComponent,
        .shininess      = m_phongExponent
    };
}

} // namespace eng::gl
