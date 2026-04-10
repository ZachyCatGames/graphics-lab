#include <engine/raytracer/eng_ObjectFactory.h>

#include <engine/raytracer/shape/shape_Triangle.h>
#include <engine/raytracer/shape/shape_Sphere.h>

namespace eng::rt {

Handle<eng::IShape> ObjectFactory::CreateSphere(const Vector3DF& pos, float radius, Handle<eng::IShader> shader) {
    return shape::Sphere::Create(pos, radius, static_cast<Handle<IShader>>(shader));
}

Handle<eng::IShape> ObjectFactory::CreateTriangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, Handle<eng::IShader> shader) {
    return shape::Triangle::Create(a, b, c, static_cast<Handle<IShader>>(shader));
}

} // namespace eng::rt
