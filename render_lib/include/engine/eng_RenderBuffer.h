#pragma once 
#include <cstddef>

namespace eng {

class RenderBuffer {
public:
    virtual ~RenderBuffer() = default;

    [[nodiscard]] virtual size_t GetWidth() const = 0;
    [[nodiscard]] virtual size_t GetHeight() const = 0;
}; // class RendeerBuffer

} // namespace eng
