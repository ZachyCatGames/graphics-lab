#pragma once
#include <engine/eng_ICamera.h>
#include <engine/eng_Ray.h>
#include <utility>

namespace eng::rt {

class ICamera : public eng::ICamera {
public:
    constexpr ICamera() = default;
    virtual ~ICamera() = default;

    virtual Ray GenerateRay(float i, float j) = 0;
private:
    /* ... */
}; // class ICamera

} // namespace eng
