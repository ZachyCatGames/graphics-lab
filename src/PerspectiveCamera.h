#pragma once
#include "ICamera.h"
#include "vec.h"

namespace eng {

class PerspectiveCamera : public ICamera {
public:
    constexpr PerspectiveCamera() : 
        m_position(), m_U(1,0,0), m_V(0,1,0), m_W(0,0,1),
        m_focal_length(1.0), m_img_plane_width(2), m_img_plane_height(2) {}

    virtual ray GenerateRay(int i, int j) override;
private:
    Vector3DF m_position;
    Vector3DF m_U, m_V, m_W;
    float m_focal_length;
    int m_img_plane_width, m_img_plane_height;
}; // class PerspectiveCamera

} // namespace eng
