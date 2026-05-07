#pragma once
#include <engine/eng_Vector3D.h>
#include <utility>

namespace eng {

class ICamera {
public:
    constexpr ICamera() = default;
    virtual ~ICamera() = default;

    virtual Vector3DF GetPosition() = 0;
    virtual Vector3DF GetDirection() = 0;

    virtual void Rotate(float pitch, float yaw) = 0;

    virtual void MoveByU(float dist) = 0;
    virtual void MoveByV(float dist) = 0;
    virtual void MoveByW(float dist) = 0;

    virtual float GetNearPlaneDistance() = 0;

    virtual std::pair<float, float> GetImageDimensions() = 0;
}; // class ICamera

} // namespace eng
