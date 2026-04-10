#pragma once
#include <engine/eng_IObjectFactory.h>

namespace eng::rt {

class ObjectFactory : public eng::IObjectFactory {
public:
    virtual Handle<eng::Scene> CreateScene(const eng::SceneConfig& conf) override;

    virtual Handle<eng::IShape> CreateSphere(const Vector3DF& pos, float radius, Handle<eng::IShader> shader) override;
    virtual Handle<eng::IShape> CreateTriangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, Handle<eng::IShader> shader) override;

    virtual Handle<eng::IShader> CreateLambertian(const std::vector<eng::shdr::PointLight>& lights) override;
    virtual Handle<eng::IShader> CreatePhong(const std::vector<eng::shdr::PointLight>& lights, float exp) override;
}; // class ObjectFactory

} // namespace eng::rt