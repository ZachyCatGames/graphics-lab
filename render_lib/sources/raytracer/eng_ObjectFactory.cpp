#include "engine/eng_ExportableRenderBuffer.h"
#include <engine/raytracer/eng_ObjectFactory.h>
#include <engine/eng_MakeShared.h>

#include <engine/raytracer/eng_PerspectiveCamera.h>

#include <engine/raytracer/shape/shape_Mesh.h>
#include <engine/raytracer/shape/shape_Triangle.h>
#include <engine/raytracer/shape/shape_Sphere.h>

#include <engine/raytracer/shader/shdr_BlinnPhong.h>
#include <engine/raytracer/shader/shdr_Lambertian.h>

#include <engine/raytracer/eng_ExportableRenderBuffer.h>

namespace eng::rt {

Handle<eng::ICamera> ObjectFactory::CreatePerspectiveCamera(const Vector3DF& pos, const Vector3DF& dir, float focal_length, int img_width, int img_height, float img_plane_height) {
    return MakeSharedPooled<PerspectiveCamera>(pos, dir, focal_length, img_width, img_height, img_plane_height);
}

Handle<eng::IShape> ObjectFactory::CreateSphere(const Vector3DF& pos, float radius, Handle<eng::IShader> shader) {
    return MakeSharedPooled<shape::Sphere>(pos, radius, shader.StaticCast<IShader>());
}

Handle<eng::IShape> ObjectFactory::CreateTriangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, Handle<eng::IShader> shader) {
    return MakeSharedPooled<shape::Triangle>(a, b, c, shader.StaticCast<IShader>());
}

Handle<eng::IShape> ObjectFactory::CreateMesh(const std::vector<Vertex>& vertices, const Vector3DF& position, Handle<eng::IShader> shader) {
    return MakeSharedPooled<shape::Mesh>(vertices, position, shader.StaticCast<IShader>());
}

Handle<eng::IShader> ObjectFactory::CreateLambertian(const Material& material, const std::vector<eng::shdr::PointLight>& lights) {
    return MakeSharedPooled<shdr::Lambertian>(material, lights);
}

Handle<eng::IShader> ObjectFactory::CreatePhong(const Material& material, const std::vector<eng::shdr::PointLight>& lights) {
    return MakeSharedPooled<shdr::BlinnPhong>(material, lights);
}

Handle<eng::Texture> ObjectFactory::CreateTexture(const float* textureData, size_t width, size_t height) {
    return nullptr;
}

Handle<eng::ExportableRenderBuffer> ObjectFactory::CreateExportableRenderBuffer(size_t width, size_t height) {
    return MakeSharedPooled<ExportableRenderBuffer>(width, height);
}

} // namespace eng::rt
