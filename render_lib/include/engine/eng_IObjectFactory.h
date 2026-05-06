#pragma once
#include "engine/eng_RenderBuffer.h"
#include <engine/eng_IShape.h>
#include <engine/eng_IShader.h>
#include <engine/eng_ExportableRenderBuffer.h>
#include <engine/eng_Scene.h>
#include <engine/eng_Texture.h>
#include <engine/shader/shdr_PointLight.h>
#include <engine/eng_Vertex.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace eng {

class IObjectFactory {
public:
    virtual ~IObjectFactory() = default;
    
    virtual Handle<eng::ICamera> CreatePerspectiveCamera(const Vector3DF& pos, const Vector3DF& dir, float focal_length, int img_width, int img_height, float img_plane_height) = 0;

    virtual Handle<IShape> CreateSphere(const Vector3DF& pos, float radius, Handle<IShader> shader) = 0;
    virtual Handle<IShape> CreateTriangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, Handle<IShader> shader) = 0;
    virtual Handle<IShape> CreateMesh(const std::vector<Vertex>& vertices, const Vector3DF& position, Handle<IShader> shader) = 0;

    virtual Handle<IShader> CreateLambertian(const Material& material, const std::vector<shdr::PointLight>& lights) = 0;
    virtual Handle<IShader> CreatePhong(const Material& material, const std::vector<shdr::PointLight>& lights) = 0;

    virtual Handle<Texture> CreateTexture(const float* textureData, size_t width, size_t height) = 0;

    virtual Handle<RenderBuffer> CreateDisplayRenderBuffer(GLFWwindow* pWindow) = 0;

    virtual Handle<ExportableRenderBuffer> CreateExportableRenderBuffer(size_t width, size_t height) = 0;
}; // class IObjectFactory

} // namespace eng
