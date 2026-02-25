#include <engine/eng_PerspectiveCamera.h>
#include <print>

namespace eng {

Ray PerspectiveCamera::GenerateRay(float i, float j) {
    const float u = m_left_bound + (m_right_bound - m_left_bound) * i / static_cast<float>(m_img_width);
    const float v = m_bottom_bound + (m_top_bound - m_bottom_bound) * j / static_cast<float>(m_img_height);

    const Vector3DF direction = -m_focal_length * m_W + u * m_U + v * m_V;

    return {m_position, direction.normalize()};
}

} // namespace eng
