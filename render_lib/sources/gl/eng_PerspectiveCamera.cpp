#include <engine/gl/eng_PerspectiveCamera.h>

#include "glm/gtc/matrix_transform.hpp"

#include <GLFW/glfw3.h>

namespace eng::gl {

glm::mat4 PerspectiveCamera::GetProjectionMatrix(float angleDegrees) const {
    // TODO: far plane?
    return glm::perspective(glm::radians(angleDegrees), m_aspect_ratio, 5.0f, -5.0f);
}

glm::mat4 PerspectiveCamera::GetViewMatrix() const {
    double camX = sin(glfwGetTime());
    double camY = cos(glfwGetTime());
    return glm::lookAt(m_position.ToGlmVector(), (m_position - m_W).ToGlmVector(), m_V.ToGlmVector());
}

} // namespace eng::gl
