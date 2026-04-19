#pragma once
#include <engine/eng_Handle.h>
#include <engine/eng_IShape.h>
#include <engine/gl/eng_BlinnPhong.h>

namespace eng::gl {

class Mesh : public IShape {
public:
    Mesh(const std::vector<float>& vbo, const std::vector<float> normals, const Vector3DF& position, Handle<IShader> shader);

    ~Mesh();

    void Render();

    virtual Vector3DF GetPosition() const override { return m_Position; }
    virtual Bounds GetBounds() const override { return Bounds(); }

    virtual Handle<IShader> GetShader() const override;
private:
    struct Vertex {
        Vector3DF pos;
        Vector3DF normal;
    }; // struct Vertex

    Handle<BlinnPhong> m_Shader;
    void* m_pGpuVertexBuffer;
    size_t m_VertexCount;
    Vector3DF m_Position;
    GLuint m_VboId;
    GLuint m_VaoId;
}; // class Mesh

} // namespace eng::gl
