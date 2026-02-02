#pragma once
#include "ray.h"

class ICamera {
public:
    constexpr ICamera() = default;
    virtual ~ICamera() = default;

    virtual ray GenerateRay(int i, int j) = 0;
private:
    /* ... */
}; // class ICamera
