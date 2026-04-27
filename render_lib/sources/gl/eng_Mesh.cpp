#include "engine/gl/eng_BlinnPhong.h"
#include <engine/gl/eng_Mesh.h>

namespace eng::gl {

Mesh::Mesh(const std::vector<Vertex>& verts, const Vector3DF& position, Handle<IShader> shader) :
    m_Shader(shader.StaticCast<BlinnPhong>()),
    m_Position(position)
{
    assert(verts.size() % 3 == 0);
    static_assert(sizeof(Vertex) == sizeof(float) * 8);

    /* Generate a VBO. */
    glGenBuffers(1, &m_VboId);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboId);

    /*
    for (auto& v : verts) {
        std::print("{} {} {}\n", v.position[0], v.position[1], v.position[2]);
        std::print("{} {}\n", v.texCoord[0], v.texCoord[1]);
    }*/

    /* Copy our vertex buffer data to the device. */
    const size_t vertexBufferSizeBytes = verts.size() * sizeof(Vertex);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferSizeBytes, verts.data(), GL_STATIC_DRAW);

    /* Unbind the vbo since we're doing with it for now... */
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* Setup our vertex array object. */
    glGenVertexArrays(1, &m_VaoId);
    glBindVertexArray(m_VaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboId);

    /* Setup attribute / location 1, our positions. */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)0);
    
    /* Setup our normal attribute. */
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)12);

    /* Setup our texture UV attribute. */
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)24);

    /* Unbind vertex array buffer. */
    glBindVertexArray(0);

    /* Unbind the vbo since we're doing with it for now... */
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::~Mesh() = default; // TODO

Handle<IShader> Mesh::GetShader() const { return m_Shader; }

void Mesh::Render() const {
    /* Activate our shader. */
    m_Shader->Activate();

    /* Bind our vertex array. */
    glBindVertexArray(m_VaoId);

    /* Show it off to the world :) */
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* Unbind our vertex array, we're done. */
    glBindVertexArray(0);

    /* Deactive our shader. */
    m_Shader->Deactivate();
}

glm::mat4 Mesh::GetModelMatrix() const {
    return glm::rotate(glm::translate(glm::mat4(1.0), m_Position.ToGlmVector()), this->rotationAngle, this->rotationAxis.ToGlmVector());
}

} // namespace eng
