#include <engine/eng_PerspectiveCamera.h>
#include <print>

namespace eng {

Ray PerspectiveCamera::GenerateRay(float i, float j) {
    const auto right_bound  = static_cast<float>(m_img_plane_width) / 2.0;
    const auto left_bound   = -right_bound;
    const auto top_bound    = static_cast<float>(m_img_plane_width) / 2.0;
    const auto bottom_bound = -top_bound;

    const float u = left_bound + (right_bound - left_bound) * static_cast<float>(i) / static_cast<float>(m_img_width);
    const float v = bottom_bound + (top_bound - bottom_bound) * static_cast<float>(j) / static_cast<float>(m_img_height);

    const Vector3DF direction = -m_focal_length * m_W + u * m_U + v * m_V;
    const Vector3DF origin    = m_position;

    return {origin, direction};
}

} // namespace eng
