#include <engine/gl/eng_Mesh.h>

namespace eng::gl {


Mesh::Mesh(const std::vector<float>& verts, const std::vector<float> normals, const Vector3DF& position, Handle<IShader> shader) :
    m_Shader(shader),
    m_Position(position)
{
    assert(verts.size() == normals.size());
    assert(verts.size() % 3 == 0);
    static_assert(sizeof(Vertex) == sizeof(float) * 6);

    /* Generate a VBO. */
    glGenBuffers(1, &m_VboId);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboId);

    std::vector<Vertex> hostVbo;
    m_VertexCount = verts.size() / 3;
    hostVbo.reserve(m_VertexCount);
    for (size_t i = 0; i < verts.size(); i += 3) {
        hostVbo.push_back({
            Vector3DF(verts[i], verts[i+1], verts[i+2]),
            Vector3DF(normals[i], normals[i+1], normals[i+2])
        });
    }

    /* Copy our vertex buffer data to the device. */
    const size_t vertexBufferSizeBytes = hostVbo.size() * sizeof(Vertex);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferSizeBytes, hostVbo.data(), GL_STATIC_DRAW);

    /* Unbind the vbo since we're doing with it for now... */
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* Setup our vertex array object. */
    glGenVertexArrays(1, &m_VaoId);
    glBindVertexArray(m_VaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboId);

    /* Setup attribute / location 1, our positions. */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);
    
    /* Setup our normal attribute. */
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)12);

    /* Unbind vertex array buffer. */
    glBindVertexArray(0);

    /* Unbind the vbo since we're doing with it for now... */
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::~Mesh() = default;

Handle<IShader> Mesh::GetShader() const { return m_Shader; }

void Mesh::Render() {
    /* Bind our vertex array. */
    glBindVertexArray(m_VaoId);

    /* Show it off to the world :) */
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* Unbind our vertex array, we're done. */
    glBindVertexArray(0);
}

} // namespace eng
