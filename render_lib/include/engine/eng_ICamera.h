#pragma once
#include "ray.h"

namespace eng {

class ICamera {
public:
    constexpr ICamera() = default;
    virtual ~ICamera() = default;

    virtual ray GenerateRay(int i, int j) = 0;
private:
    /* ... */
}; // class ICamera

} // namespace eng
