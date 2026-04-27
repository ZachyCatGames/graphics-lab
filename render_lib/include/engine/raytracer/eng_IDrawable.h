#pragma once
#include <engine/eng_RenderBuffer.h>

namespace eng::rt {

class IDrawable {
public:
    virtual void SetPixelColor(size_t x, size_t y, Vector3DF color) = 0; 
}; // class RenderBufferBase

} // namespace eng::rt
