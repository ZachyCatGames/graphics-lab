#include "engine/eng_RenderBuffer.h"
#include <engine/gl/eng_GlfwWindow.h>
#include <cstdlib>

namespace eng::gl {

namespace {

constinit bool g_glfwInitialized = false;

void InitializeGlfw() {
    if (!glfwInit())
        exit(-1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    g_glfwInitialized = true;
}

} // namespace

GlfwWindow::GlfwWindow(GLFWwindow* pWindow)
{
    m_pWindow = pWindow;
    glfwGetFramebufferSize(m_pWindow, &m_width, &m_height);
}

void GlfwWindow::SetContextCurrent() {
    glfwMakeContextCurrent(m_pWindow);
}

void GlfwWindow::NotifyRenderDone() {
    glfwSwapBuffers(m_pWindow);
}

size_t GlfwWindow::GetWidth() const { return m_width; }
size_t GlfwWindow::GetHeight() const { return m_height; }

} // namespace eng::gl
