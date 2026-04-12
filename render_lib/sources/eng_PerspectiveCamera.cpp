#include <engine/eng_PerspectiveCamera.h>
#include <print>

#ifdef ENGINE_BUILD_GL_RENDER
#include "glm/gtc/matrix_transform.hpp"
#endif // ENGINE_BUILD_GL_RENDER

namespace eng {

#ifdef ENGINE_BUILD_GL_RENDER

glm::mat4 PerspectiveCamera::GetProjectionMatrix(float angleDegrees) const {
    // TODO: far plane?
    return glm::perspective(glm::radians(angleDegrees), m_aspect_ratio, 5.0f, -5.0f);
}

glm::mat4 PerspectiveCamera::GetViewMatrix() const {
    return glm::lookAt(m_position.ToGlmVector(), (m_position - m_W).ToGlmVector(), m_V.ToGlmVector());
}

#endif // ENGINE_BUILD_GL_RENDER

} // namespace eng
