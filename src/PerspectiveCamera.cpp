#include "PerspectiveCamera.h"

namespace eng {

ray PerspectiveCamera::GenerateRay(int i, int j) {
    const auto right_bound  = static_cast<float>(m_img_plane_width) / 2;
    const auto left_bound   = -right_bound;
    const auto top_bound    = static_cast<float>(m_img_plane_width) / 2;
    const auto bottom_bound = -top_bound;

    const float u = left_bound + (right_bound - left_bound) * (static_cast<float>(i) + 0.5);
    const float v = bottom_bound + (top_bound - bottom_bound) * (static_cast<float>(j) + 0.5);

    const Vector3DF direction = -m_focal_length * m_W + u * m_U + v * m_V;
    const Vector3DF origin    = m_position;

    return {origin, direction};
}

} // namespace eng
