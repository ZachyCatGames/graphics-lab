#pragma once
#include <engine/eng_ICamera.h>

namespace eng {

template<typename Interface>
class CameraBase : public Interface {
public:
    virtual Vector3DF GetPosition() override { return m_position; }
    virtual Vector3DF GetDirection() override { return m_W; }

    virtual float GetNearPlaneDistance() override { return m_focal_length; }

    virtual std::pair<float, float> GetImageDimensions() override { return { m_img_width, m_img_height }; }

public:
    constexpr CameraBase() : 
        m_position(), m_U(1,0,0), m_V(0,1,0), m_W(0,0,1),
        m_focal_length(1.0), m_img_plane_width(2), m_img_plane_height(2) {}

    constexpr CameraBase(const Vector3DF& pos, const Vector3DF& dir, float focal_length, int img_width, int img_height, float img_plane_height) :
        m_position(pos), m_img_width(img_width), m_img_height(img_height), m_focal_length(focal_length), m_img_plane_height(img_plane_height)
    {
        /* Calculate plane width. */
        m_aspect_ratio = static_cast<float>(m_img_width) / static_cast<float>(m_img_height);
        m_img_plane_width = m_img_plane_height * m_aspect_ratio;

        /* W is just the direction normalized, since W _is_ the direction we're facing. */
        m_W = -dir.normalize();

        /* V points to our right. We'll use a cross product with "up" to get right. We don't yet have an up, so use absolute up for now. */
        {
            static constexpr Vector3DF b {0, 1, 0};
            m_U = cross(b, m_W).normalize();
        }

        /* U points up, take a cross product with our forward and right to get up. */
        m_V = cross(m_W, m_U).normalize();

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

protected:
    Vector3DF m_position;
    Vector3DF m_U, m_V, m_W;
    int m_img_width, m_img_height;
    float m_aspect_ratio;
    float m_focal_length;
    float m_img_plane_width, m_img_plane_height;
    float m_right_bound, m_left_bound, m_top_bound, m_bottom_bound;
}; // class CameraBase

} // namespace eng
