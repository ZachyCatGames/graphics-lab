#include "engine/eng_ICamera.h"
#include "engine/eng_IRenderer.h"
#include "engine/gl/eng_PerspectiveCamera.h"
#include <engine/gl/eng_IRenderable.h>
#include <engine/gl/eng_Renderer.h>
#include <engine/gl/eng_BlinnPhong.h>
#include <engine/gl/eng_ICamera.h>
#include <engine/gl/eng_Mesh.h>

#include <GL/glew.h>
#include <print>

namespace eng::gl {

namespace {

constinit Handle<IRenderable> g_currentRenderBuffer = nullptr;

} // namespace

void Renderer::PreRender() {
    /* ... */
}

void Renderer::Render(std::string_view cameraName, Handle<RenderBuffer> fb) {
    /* Find the right camera. */
    Handle<ICamera> camera = m_Scene->cameras.Get(cameraName).StaticCast<ICamera>();

    /* Get camera position (eye position), projection matrix, and view matrix. */
    Vector3DF eyePosition = camera->GetPosition();
    glm::mat4 projMatrix  = camera->GetProjectionMatrix(45.0f);
    glm::mat4 viewMatrix  = camera->GetViewMatrix();

    /* Check if the provided render buffer matches the current render buffer. */
    if (!fb.IsValid()) {
        std::print("Invalid renderbuffer object passed to renderer!\n");
        exit(-1);
    }
    Handle<IRenderable> renderable = fb.DynamicCast<IRenderable>();
    if (renderable == g_currentRenderBuffer) {
        g_currentRenderBuffer = renderable;
        renderable->SetContextCurrent();
    }

    if (m_ClearFrambuffer) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    /* Iterate over each shape. */
    for (const auto& shape : m_Scene->shapes.GetList()) {
        Handle<Mesh> mesh = shape.StaticCast<Mesh>();

        /* Generate the model matrix. */
        glm::mat4 modelMatrix  = mesh->GetModelMatrix();
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));

        /* Setup its shader. */
        /* NOTE: we only support BlinnPhong atm. */
        Handle<BlinnPhong> shader = mesh->GetShader().StaticCast<BlinnPhong>();
        shader->AssignProjectionMatrix(projMatrix);
        shader->AssignViewMatrix(viewMatrix);
        shader->AssignModelMatrix(modelMatrix);
        shader->AssignNormalMatrix(normalMatrix);
        shader->AssignEyePosition(Vector4DF(eyePosition, 1.0f));

        /* Render the mesh. */
        mesh->Render();
    }

}

} // namespace eng::gl
