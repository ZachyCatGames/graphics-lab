#pragma once
#include <cstddef>

namespace eng {

class ReferenceCount {
public:
    constexpr ReferenceCount() : m_count(0) {}
    constexpr ReferenceCount(const size_t count) : m_count(count) {}

    constexpr void Increment() noexcept { m_count++; }
    constexpr bool Decrement() noexcept { return --m_count == 0; }

    [[nodiscard]] constexpr auto GetCount() const noexcept { return m_count; }

    [[nodiscard]] constexpr bool IsZero() const noexcept { return m_count == 0; }
private:
    size_t m_count;
}; // class ReferenceCount

} // namespace eng
