#pragma once
#include <engine/eng_Ray.h>

namespace eng {

class ICamera {
public:
    constexpr ICamera() = default;
    virtual ~ICamera() = default;

    virtual Ray GenerateRay(int i, int j) = 0;
private:
    /* ... */
}; // class ICamera

} // namespace eng
