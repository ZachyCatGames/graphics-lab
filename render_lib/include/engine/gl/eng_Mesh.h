#pragma once
#include <engine/eng_Handle.h>
#include <engine/eng_IShape.h>
#include <engine/eng_Material.h>
#include <engine/eng_ObjectBase.h>
#include <engine/eng_Vertex.h>
#include <engine/gl/eng_BlinnPhong.h>

namespace eng::gl {

class Mesh : public IShape, public ObjectBase<Mesh> {
public:
    Mesh(const std::vector<Vertex>& vbo, const Vector3DF& position, Handle<IShader> shader);

    ~Mesh();

    void Render() const;

    virtual Vector3DF GetPosition() const override { return m_Position; }
    virtual Bounds GetBounds() const override { return Bounds(); }

    virtual Handle<IShader> GetShader() const override;

    glm::mat4 GetModelMatrix() const;
public:
    /**************************************
     * Test Functions
     **************************************/
    constexpr GLuint GetVertexBufferObjectId() const noexcept { return m_VboId; }
    constexpr GLuint GetVertexArrayObjectId() const noexcept { return m_VaoId; }
private:
    Handle<BlinnPhong> m_Shader;

    GLuint m_VboId;
    GLuint m_VaoId;

    size_t m_VertexCount;
    Vector3DF m_Position;
}; // class Mesh

} // namespace eng::gl
