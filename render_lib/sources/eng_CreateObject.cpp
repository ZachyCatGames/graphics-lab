#include <engine/eng_CreateObject.h>
#include <engine/detail/eng_CreateObject.h>

namespace eng {

namespace {

std::unique_ptr<IObjectFactory> g_shapeFactory;

} // namespace

namespace detail {

void InitializeObjectFactory(std::unique_ptr<IObjectFactory>&& shapeFactory) {
    g_shapeFactory = std::move(shapeFactory);
}

} // namespace detail

Handle<IShape> CreateSphere(const Vector3DF& pos, float radius, Handle<eng::IShader> shader) {
    return g_shapeFactory->CreateSphere(pos, radius, std::move(shader));
}

Handle<IShape> CreateTriangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, Handle<eng::IShader> shader) {
    return g_shapeFactory->CreateTriangle(a, b, c, std::move(shader));
}

} // namespace eng
