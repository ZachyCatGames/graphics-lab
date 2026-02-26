#pragma once
#include <engine/eng_Ray.h>

namespace eng {

class ICamera {
public:
    constexpr ICamera() = default;
    virtual ~ICamera() = default;

    virtual Ray GenerateRay(float i, float j) = 0;

    constexpr virtual float GetMinT() const = 0;
private:
    /* ... */
}; // class ICamera

} // namespace eng
