#include <engine/eng_PerspectiveCamera.h>
#include <print>

#ifdef ENGINE_BUILD_GL_RENDER
#include "glm/gtc/matrix_transform.hpp"
#endif // ENGINE_BUILD_GL_RENDER

namespace eng {

Ray PerspectiveCamera::GenerateRay(float i, float j) {
    const float u = m_left_bound + (m_right_bound - m_left_bound) * i / static_cast<float>(m_img_width);
    const float v = m_bottom_bound + (m_top_bound - m_bottom_bound) * j / static_cast<float>(m_img_height);

    const Vector3DF direction = -m_focal_length * m_W + u * m_U + v * m_V;

    return {m_position, direction.normalize()};
}

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
