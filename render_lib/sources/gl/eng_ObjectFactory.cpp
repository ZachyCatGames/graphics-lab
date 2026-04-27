#include <engine/gl/eng_ObjectFactory.h>
#include <engine/eng_MakeShared.h>

#include <engine/gl/eng_PerspectiveCamera.h>

#include <engine/gl/eng_Mesh.h>
#include <engine/gl/eng_Sphere.h>

#include <engine/gl/eng_BlinnPhong.h>

#include <engine/gl/eng_Texture.h>

namespace eng::gl {

Handle<eng::ICamera> ObjectFactory::CreatePerspectiveCamera(const Vector3DF& pos, const Vector3DF& dir, float focal_length, int img_width, int img_height, float img_plane_height) {
    return MakeSharedPooled<PerspectiveCamera>(pos, dir, focal_length, img_width, img_height, img_plane_height);
}

Handle<IShape> ObjectFactory::CreateSphere(const Vector3DF& pos, float radius, Handle<IShader> shader) {
    return MakeSharedPooled<Sphere>(radius, pos, std::move(shader));
}

Handle<IShape> ObjectFactory::CreateTriangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, Handle<IShader> shader) {
    return nullptr;
}

Handle<IShape> ObjectFactory::CreateMesh(const std::vector<Vertex>& vertices, const Vector3DF& position, Handle<IShader> shader) {
    return MakeSharedPooled<Mesh>(vertices, position, shader);
}

Handle<IShader> ObjectFactory::CreateLambertian(const Material& material, const std::vector<shdr::PointLight>& lights) {
    return nullptr;
}

Handle<IShader> ObjectFactory::CreatePhong(const Material& material, const std::vector<shdr::PointLight>& lights) {
    return MakeSharedPooled<BlinnPhong>(material);
}

Handle<eng::Texture> ObjectFactory::CreateTexture(const float* textureData, size_t width, size_t height) {
    return MakeSharedPooled<Texture>(textureData, width, height);
}

Handle<eng::ExportableRenderBuffer> ObjectFactory::CreateExportableRenderBuffer(size_t width, size_t height) {
    return nullptr;
}

} // namespace eng::gl
