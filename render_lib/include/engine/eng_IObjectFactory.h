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
    
    virtual Handle<eng::ICamera> CreatePerspectiveCamera(const Vector3DF& pos, const Vector3DF& dir, float focal_length, int img_width, int img_height, float img_plane_height) = 0;

    virtual Handle<IShape> CreateSphere(const Vector3DF& pos, float radius, Handle<IShader> shader) = 0;
    virtual Handle<IShape> CreateTriangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, Handle<IShader> shader) = 0;
    virtual Handle<IShape> CreateMesh(const std::vector<float>& vertices, const Vector3DF& position, Handle<IShader> shader) = 0;

    virtual Handle<IShader> CreateLambertian(Vector3DF baseColor, const std::vector<shdr::PointLight>& lights) = 0;
    virtual Handle<IShader> CreatePhong(Vector3DF baseColor, const std::vector<shdr::PointLight>& lights, float exp) = 0;
}; // class IObjectFactory

} // namespace eng
