#pragma once
#include "ICamera.h"
#include "vec.h"

namespace eng {

class PerspectiveCamera : public ICamera {
public:
    constexpr PerspectiveCamera() : 
        m_position(), m_U(1,0,0), m_V(0,1,0), m_W(0,0,1),
        m_focal_length(1.0), m_img_plane_width(2), m_img_plane_height(2) {}

    constexpr PerspectiveCamera(const Vector3DF& pos, const Vector3DF& dir, float focal_length, int img_width, int img_height, float img_plane_height) :
        m_position(pos), m_U(), m_V(), m_W(), m_img_width(img_width), m_img_height(img_height), m_focal_length(focal_length), m_img_plane_width(0), m_img_plane_height(img_plane_height)
    {
        /* Calculate plane width. */
        const auto aspect_ratio = static_cast<float>(m_img_width) / static_cast<float>(m_img_height);
        m_img_plane_width = m_img_plane_height * aspect_ratio;

        /* W is just the direction normalized, since W _is_ the direction we're facing. */
        m_W = dir.normalize();

        /* V points to our right. We'll use a cross product with "up" to get right. We don't yet have an up, so use absolute up for now. */
        {
            static constexpr Vector3DF b {0, 1, 0};
            m_U = cross(m_W, b).normalize();
        }

        /* U points up, take a cross product with our forward and right to get up. */
        m_V = cross(m_U, m_W).normalize();
    }

    virtual ~PerspectiveCamera() = default;

    virtual ray GenerateRay(int i, int j) override;
private:
    Vector3DF m_position;
    Vector3DF m_U, m_V, m_W;
    int m_img_width, m_img_height;
    float m_focal_length;
    float m_img_plane_width, m_img_plane_height;
}; // class PerspectiveCamera

} // namespace eng
