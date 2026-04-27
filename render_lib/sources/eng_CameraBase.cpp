#include <engine/eng_CameraBase.h>

namespace eng::detail {

CameraBaseImpl::CameraBaseImpl() : 
    m_position(), m_U(1,0,0), m_V(0,1,0), m_W(0,0,1),
    m_focal_length(1.0), m_img_plane_width(2), m_img_plane_height(2) {}

CameraBaseImpl::CameraBaseImpl(const Vector3DF& pos, const Vector3DF& dir, float focal_length, int img_width, int img_height, float img_plane_height) :
    m_position(pos), m_img_width(img_width), m_img_height(img_height), m_focal_length(focal_length), m_img_plane_height(img_plane_height)
{
    /* Calculate plane width. */
    m_aspect_ratio = static_cast<float>(m_img_width) / static_cast<float>(m_img_height);
    m_img_plane_width = m_img_plane_height * m_aspect_ratio;

    /* W is just the direction normalized, since W _is_ the direction we're facing. */
    m_W = -dir.normalize();

    this->ComputeUV();

    if not consteval {
        std::cout << m_W << '\n';
        std::cout << m_U << '\n';
        std::cout << m_V << '\n';
    }

    m_right_bound  = static_cast<float>(m_img_plane_width) / 2.0;
    m_left_bound   = -m_right_bound;
    m_top_bound    = static_cast<float>(m_img_plane_width) / 2.0;
    m_bottom_bound = -m_top_bound;
}

void CameraBaseImpl::ComputeUV() {
    /* V points to our right. We'll use a cross product with "up" to get right. We don't yet have an up, so use absolute up for now. */
    {
        static constexpr Vector3DF up {0, 1, 0};
        m_U = cross(up, m_W).normalize();
    }

    /* U points up, take a cross product with our forward and right to get up. */
    m_V = cross(m_W, m_U).normalize();
}

void CameraBaseImpl::Rotate(float pitch, float yaw) {
    m_W[0] = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    m_W[1] = sinf(glm::radians(pitch));
    m_W[2] = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    m_W = m_W.normalize();
    this->ComputeUV();
}

} // namespace eng::detail
