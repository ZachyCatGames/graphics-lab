#pragma once
#include <engine/eng_Engine.h>
#include <engine/eng_IObjectFactory.h>

namespace eng::rt {

class ObjectFactory : public eng::IObjectFactory {
public:
    constexpr ObjectFactory(Engine* pEng) : m_pEngine(pEng) {}

    virtual Handle<eng::ICamera> CreatePerspectiveCamera(const Vector3DF& pos, const Vector3DF& dir, float focal_length, int img_width, int img_height, float img_plane_height) override;

    virtual Handle<eng::IShape> CreateSphere(const Vector3DF& pos, float radius, Handle<eng::IShader> shader) override;
    virtual Handle<eng::IShape> CreateTriangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, Handle<eng::IShader> shader) override;
    virtual Handle<eng::IShape> CreateMesh(const std::vector<float>& vertices, const Vector3DF& position, Handle<eng::IShader> shader) override;

    virtual Handle<eng::IShader> CreateLambertian(Vector3DF baseColor, const std::vector<eng::shdr::PointLight>& lights) override;
    virtual Handle<eng::IShader> CreatePhong(Vector3DF baseColor, const std::vector<eng::shdr::PointLight>& lights, float exp) override;
private:
    Engine* m_pEngine;
}; // class ObjectFactory

} // namespace eng::rt