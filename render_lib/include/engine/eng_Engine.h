#pragma once
#include "engine/eng_RenderBuffer.h"
#include <engine/eng_Handle.h>
#include <engine/eng_HandleGraphicsArgs.h>
#include <engine/eng_IObjectFactory.h>
#include <engine/eng_IRenderer.h>
#include <engine/eng_ObjectBase.h>
#include <engine/eng_Scene.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

namespace eng {

class Engine {
public:
    int Initialize(int argc, char** argv);

    [[nodiscard]] std::string GetRenderString() const { return m_renderMode; }

    [[nodiscard]] const GraphicsArgs& GetArguments() const { return m_gArgs; }

    /** Window APIs. */
    [[nodiscard]] GLFWwindow* GetGlfwWindow() const { return m_pWindow; }
    [[nodiscard]] bool ShouldClose() const { return glfwWindowShouldClose(m_pWindow); }


    /* NOTE: This is for debugging!!! */
    [[nodiscard]] IRenderer* GetRenderer() const { return m_pRenderer.get(); }

    /* Scene management. */
    [[nodiscard]] const auto& GetActiveScene() const { return m_pActiveScene; }

    void SetActiveScene(std::shared_ptr<Scene>& pScene) { m_pActiveScene = std::move(pScene); }

    std::shared_ptr<Scene> LoadSceneFromJson(std::string_view jsonPath, bool assignActive = false);

    void RenderActiveScene(std::string_view cameraName, const Handle<RenderBuffer>& fb) {
        m_pRenderer->PreRender();

        m_pRenderer->Render(cameraName, fb);
    }

    /* Renderbuffer management. */
    const Handle<RenderBuffer>& GetDisplayRenderBuffer() {
        return m_displayRenderBuffer;
    }

    /* Texture creation. */
    Handle<Texture> OpenTextureFromPNG(std::string_view path);

    /* Object creation wrappers. */
    Handle<eng::ICamera> CreatePerspectiveCamera(const Vector3DF& pos, const Vector3DF& dir, float focal_length, int img_width, int img_height, float img_plane_height) {
        return m_pObjFactory->CreatePerspectiveCamera(pos, dir, focal_length, img_width, img_height, img_plane_height);
    }

    Handle<IShape> CreateSphere(const Vector3DF& pos, float radius, const Handle<IShader>& shader) {
        return m_pObjFactory->CreateSphere(pos, radius, shader);
    }
    Handle<IShape> CreateTriangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, const Handle<IShader>& shader) {
        return m_pObjFactory->CreateTriangle(a, b, c, shader);
    }
    Handle<IShape> CreateMesh(const std::vector<Vertex>& vertices, const Vector3DF& position, const Handle<IShader>& shader) {
        return m_pObjFactory->CreateMesh(vertices, position, shader);
    }

    Handle<IShader> CreateLambertian(const Material& material) {
        return m_pObjFactory->CreateLambertian(material);
    }
    Handle<IShader> CreatePhong(const Material& material) {
        return m_pObjFactory->CreatePhong(material);
    }
    Handle<IShader> CreateMirror() {
        return m_pObjFactory->CreateMirror();
    }
    Handle<IShader> CreateDiffuseShader(const Material& material) {
        return m_pObjFactory->CreateDiffuseShader(material);
    }
    Handle<IShader> CreateEmitter(const Material& material) {
        return m_pObjFactory->CreateEmitter(material);
    }

    Handle<Texture> CreateTexture(const float* textureData, size_t width, size_t height) {
        return m_pObjFactory->CreateTexture(textureData, width, height);
    }

    Handle<ExportableRenderBuffer> CreateExportableRenderBuffer(size_t width, size_t height) {
        return m_pObjFactory->CreateExportableRenderBuffer(width, height);
    }
private:
    void InitializeOpenGL();
private:
    GraphicsArgs m_gArgs;

    std::shared_ptr<Scene> m_pActiveScene;

    std::unique_ptr<IObjectFactory> m_pObjFactory;
    std::unique_ptr<IRenderer> m_pRenderer;

    GLFWwindow* m_pWindow;
    Handle<RenderBuffer> m_displayRenderBuffer;

    std::string m_renderMode;
}; // class Engine

} // namespace eng
