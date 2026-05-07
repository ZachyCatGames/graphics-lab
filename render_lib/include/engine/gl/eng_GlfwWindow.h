#pragma once
#include <engine/eng_RenderBuffer.h>
#include <engine/gl/eng_IRenderable.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace eng::gl {

class GlfwWindow : public IRenderable {
public:
    GlfwWindow(GLFWwindow* pWindow);

    void SetContextCurrent() override;
    void NotifyRenderDone() override;

    [[nodiscard]] size_t GetWidth() const override;
    [[nodiscard]] size_t GetHeight() const override;
private:
    GLFWwindow* m_pWindow;
    int m_width, m_height;
}; // class GlfwWindow

} // namespace eng::gl
