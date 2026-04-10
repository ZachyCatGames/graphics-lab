#pragma once
#include <engine/eng_IObjectFactory.h>

namespace eng::rt {

class ObjectFactory : public eng::IObjectFactory {
public:
    virtual Handle<eng::IShape> CreateSphere(const Vector3DF& pos, float radius, Handle<eng::IShader> shader) override;
    virtual Handle<eng::IShape> CreateTriangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, Handle<eng::IShader> shader) override;
}; // class ObjectFactory

} // namespace eng::rt