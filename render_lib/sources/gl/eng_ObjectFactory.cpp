#include "engine/gl/eng_GlfwWindow.h"
#include <engine/gl/eng_ObjectFactory.h>
#include <engine/eng_MakeShared.h>

#include <engine/gl/eng_PerspectiveCamera.h>

#include <engine/gl/eng_Mesh.h>
#include <engine/gl/eng_Sphere.h>

#include <engine/gl/eng_BlinnPhong.h>

#include <engine/gl/eng_Texture.h>

#include <engine/gl/eng_GlfwWindow.h>

namespace eng::gl {

Handle<eng::ICamera> ObjectFactory::CreatePerspectiveCamera(const Vector3DF& pos, const Vector3DF& dir, float focal_length, int img_width, int img_height, float img_plane_height) {
    return MakeSharedPooled<PerspectiveCamera>(pos, dir, focal_length, img_width, img_height, img_plane_height);
}

Handle<IShape> ObjectFactory::CreateSphere(const Vector3DF& pos, float radius, Handle<IShader> shader) {
    return MakeSharedPooled<Sphere>(radius, pos, std::move(shader));
}

Handle<IShape> ObjectFactory::CreateTriangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, Handle<IShader> shader) {
    std::vector<Vertex> verts {
        {
            .position = a,
            .normal   = cross(b, c)
        },
        {
            .position = b,
            .normal   = cross(c, a)
        },
        {
            .position = c,
            .normal   = cross(a, b)
        }
    };
    /* TODO: this should probably take a position vector. */
    return this->CreateMesh(verts, {0,0,0}, shader);
}

Handle<IShape> ObjectFactory::CreateMesh(const std::vector<Vertex>& vertices, const Vector3DF& position, Handle<IShader> shader) {
    return MakeSharedPooled<Mesh>(vertices, position, shader);
}

Handle<IShader> ObjectFactory::CreateLambertian(const Material& material) {
    /* Force specular to 0 and shininess to 1, this will effectively disable specular reflections. */
    Material mat {
        .texture      = material.texture,
        .ambientLight = material.ambientLight,
        .diffuse      = material.diffuse,
        .specular     = Vector3DF(0,0,0),
        .shininess    = 1
    };
    return MakeSharedPooled<BlinnPhong>(mat);
}

Handle<IShader> ObjectFactory::CreatePhong(const Material& material) {
    return MakeSharedPooled<BlinnPhong>(material);
}

Handle<IShader> ObjectFactory::CreateMirror() {
    /* This should be a shiny object, currently we don't do reflections... */
    static constexpr Material material {
        .ambientLight = {0.5, 0.5, 0.5},
        .diffuse   = {0,0,0},
        .specular  = {1,1,1},
        .shininess = 400
    };

    return this->CreatePhong(material);
}

Handle<IShader> ObjectFactory::CreateDiffuseShader(const Material& material) {
    return this->CreateLambertian(material);
}

Handle<IShader> ObjectFactory::CreateEmitter(const Material& material) {
    return this->CreatePhong({
        .texture = material.texture,
        .ambientLight = material.ambientLight,
        .diffuse = {0,0,0},
        .specular = {0,0,0},
        .shininess = 1
    });
}

Handle<eng::Texture> ObjectFactory::CreateTexture(const float* textureData, size_t width, size_t height) {
    return MakeSharedPooled<Texture>(textureData, width, height);
}

Handle<eng::RenderBuffer> ObjectFactory::CreateDisplayRenderBuffer(GLFWwindow* pWindow) {
    return MakeSharedPooled<GlfwWindow>(pWindow);
}

Handle<eng::ExportableRenderBuffer> ObjectFactory::CreateExportableRenderBuffer(size_t width, size_t height) {
    assert(0);
    return nullptr;
}

} // namespace eng::gl
