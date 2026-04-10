#include <engine/raytracer/eng_ObjectFactory.h>

#include <engine/raytracer/eng_Scene.h>

#include <engine/raytracer/shape/shape_Triangle.h>
#include <engine/raytracer/shape/shape_Sphere.h>

#include <engine/raytracer/shader/shdr_BlinnPhong.h>
#include <engine/raytracer/shader/shdr_Lambertian.h>

namespace eng::rt {

Handle<eng::Scene> ObjectFactory::CreateScene(const eng::SceneConfig& conf) {
    return Scene::Create(conf.rtConfig.recursionDepth);
}

Handle<eng::IShape> ObjectFactory::CreateSphere(const Vector3DF& pos, float radius, Handle<eng::IShader> shader) {
    return shape::Sphere::Create(pos, radius, static_cast<Handle<IShader>>(shader));
}

Handle<eng::IShape> ObjectFactory::CreateTriangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, Handle<eng::IShader> shader) {
    return shape::Triangle::Create(a, b, c, static_cast<Handle<IShader>>(shader));
}

Handle<eng::IShader> ObjectFactory::CreateLambertian(const std::vector<eng::shdr::PointLight>& lights) {
    return shdr::Lambertian::Create(nullptr, lights);
}

Handle<eng::IShader> ObjectFactory::CreatePhong(const std::vector<eng::shdr::PointLight>& lights, float exp) {
    return shdr::BlinnPhong::Create(nullptr, lights, exp);
}

} // namespace eng::rt
