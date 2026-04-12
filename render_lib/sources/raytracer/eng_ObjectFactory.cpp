#include <engine/raytracer/eng_ObjectFactory.h>

#include <engine/raytracer/eng_PerspectiveCamera.h>

#include <engine/raytracer/shape/shape_Triangle.h>
#include <engine/raytracer/shape/shape_Sphere.h>

#include <engine/raytracer/shader/shdr_BlinnPhong.h>
#include <engine/raytracer/shader/shdr_Lambertian.h>

namespace eng::rt {

Handle<eng::ICamera> ObjectFactory::CreatePerspectiveCamera(const Vector3DF& pos, const Vector3DF& dir, float focal_length, int img_width, int img_height, float img_plane_height) {
    return PerspectiveCamera::Create(pos, dir, focal_length, img_width, img_height, img_plane_height);
}

Handle<eng::IShape> ObjectFactory::CreateSphere(const Vector3DF& pos, float radius, Handle<eng::IShader> shader) {
    return shape::Sphere::Create(pos, radius, static_cast<Handle<IShader>>(shader));
}

Handle<eng::IShape> ObjectFactory::CreateTriangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, Handle<eng::IShader> shader) {
    return shape::Triangle::Create(a, b, c, static_cast<Handle<IShader>>(shader));
}

Handle<eng::IShader> ObjectFactory::CreateLambertian(Vector3DF baseColor, const std::vector<eng::shdr::PointLight>& lights) {
    return shdr::Lambertian::Create(baseColor, lights);
}

Handle<eng::IShader> ObjectFactory::CreatePhong(Vector3DF baseColor, const std::vector<eng::shdr::PointLight>& lights, float exp) {
    return shdr::BlinnPhong::Create(baseColor, lights, exp);
}

} // namespace eng::rt
