#pragma once
#include <engine/eng_IShape.h>
#include <engine/eng_IShader.h>
#include <engine/eng_ObjectManager.h>
#include <engine/eng_Scene.h>
#include <engine/shader/shdr_PointLight.h>

namespace eng {

class IObjectFactory {
public:
    virtual ~IObjectFactory() = default;
    
    virtual Handle<eng::Scene> CreateScene(const SceneConfig& conf) = 0;

    virtual Handle<IShape> CreateSphere(const Vector3DF& pos, float radius, Handle<IShader> shader) = 0;
    virtual Handle<IShape> CreateTriangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, Handle<IShader> shader) = 0;

    virtual Handle<IShader> CreateLambertian(const std::vector<shdr::PointLight>& lights) = 0;
    virtual Handle<IShader> CreatePhong(const std::vector<shdr::PointLight>& lights, float exp) = 0;
}; // class IObjectFactory

} // namespace eng
