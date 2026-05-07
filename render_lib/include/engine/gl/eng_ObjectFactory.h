#pragma once
#include <engine/eng_IObjectFactory.h>

namespace eng::gl {

class ObjectFactory : public IObjectFactory {
public:
    virtual ~ObjectFactory() = default;
    
    Handle<eng::ICamera> CreatePerspectiveCamera(const Vector3DF& pos, const Vector3DF& dir, float focal_length, int img_width, int img_height, float img_plane_height) override;

    Handle<IShape> CreateSphere(const Vector3DF& pos, float radius, Handle<IShader> shader) override;
    Handle<IShape> CreateTriangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, Handle<IShader> shader) override;
    Handle<IShape> CreateMesh(const std::vector<Vertex>& vertices, const Vector3DF& position, Handle<IShader> shader) override;

    Handle<IShader> CreateLambertian(const Material& material) override;
    Handle<IShader> CreatePhong(const Material& material) override;
    Handle<eng::IShader> CreateMirror() override;
    Handle<eng::IShader> CreateDiffuseShader(const Material& material) override;
    Handle<eng::IShader> CreateEmitter(const Material& material) override;

    Handle<eng::Texture> CreateTexture(const float* textureData, size_t width, size_t height) override;

    Handle<eng::RenderBuffer> CreateDisplayRenderBuffer(GLFWwindow* pWindow) override;

    Handle<eng::ExportableRenderBuffer> CreateExportableRenderBuffer(size_t width, size_t height) override;
}; // class ObjectFactory

} // namespace eng::gl
