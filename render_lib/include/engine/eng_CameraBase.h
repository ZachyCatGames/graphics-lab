#pragma once
#include <engine/eng_ICamera.h>

namespace eng {

namespace detail {

class CameraBaseImpl {
public:
    CameraBaseImpl();

    CameraBaseImpl(const Vector3DF& pos, const Vector3DF& dir, float focal_length, int img_width, int img_height, float img_plane_height);

    void ComputeUV();

    void RotateImpl(float pitch, float yaw);

    void MoveByUImpl(float dist) { m_position += m_U * dist; }
    void MoveByVImpl(float dist) { m_position += m_V * dist; }
    void MoveByWImpl(float dist) { m_position += m_W * dist; }
protected:
    Vector3DF m_position;
    Vector3DF m_U, m_V, m_W;
    int m_img_width, m_img_height;
    float m_aspect_ratio;
    float m_focal_length;
    float m_img_plane_width, m_img_plane_height;
    float m_right_bound, m_left_bound, m_top_bound, m_bottom_bound;
}; // class CameraBaseImpl

} // namespace detail

template<typename Interface>
class CameraBase : public detail::CameraBaseImpl, public Interface {
public:
    Vector3DF GetPosition() override { return m_position; }
    Vector3DF GetDirection() override { return m_W; }

    void Rotate(float pitch, float yaw) override { this->RotateImpl(pitch, yaw); }

    void MoveByU(float dist) override { this->MoveByUImpl(dist); }
    void MoveByV(float dist) override { this->MoveByVImpl(dist); }
    void MoveByW(float dist) override { this->MoveByWImpl(dist); }

    float GetNearPlaneDistance() override { return m_focal_length; }

    std::pair<float, float> GetImageDimensions() override { return { m_img_width, m_img_height }; }
public:
    using detail::CameraBaseImpl::CameraBaseImpl;
}; // class CameraBase

} // namespace eng
