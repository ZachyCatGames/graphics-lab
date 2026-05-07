#pragma once
#include <cstddef>

namespace eng {

class Texture {
public:
    constexpr Texture(size_t width, size_t height) :
        m_width(width), m_height(height) {}

    virtual ~Texture() = default;

    [[nodiscard]] constexpr size_t GetWidth() const noexcept { return m_width; }
    [[nodiscard]] constexpr size_t GetHeight() const noexcept { return m_height; }
private:
    size_t m_width, m_height;
}; // class Texture

} // namespace eng
