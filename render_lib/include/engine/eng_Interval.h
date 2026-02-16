#pragma once

namespace eng {

template<typename T>
class Interval {
public:
    using value_type = std::remove_cvref_t<T>;
public:
    constexpr Interval() : m_min(0.0), m_max(1.0) {}

    constexpr Interval(value_type min, value_type max) : m_min(min), m_max(max) {}

    [[nodiscard]] constexpr auto Min() const { return m_min; }
    [[nodiscard]] constexpr auto Max() const { return m_max; }

    [[nodiscard]] constexpr bool Contains(value_type val) const {
        return val <= m_max && val >= m_min;
    }

    [[nodiscard]] constexpr bool Surrounds(value_type val) const {
        return val < m_max && val > m_min;
    }
private:
    value_type m_min, m_max;
}; // class Interval

} // namespace eng
