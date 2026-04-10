#pragma once
#include <atomic>
#include <cstddef>

namespace eng {

class ReferenceCount {
private:
    union CounterValue {
        constexpr CounterValue() noexcept {
            if consteval {
                non_atomic = 0;
            } else {
                atomic = 0;
            }
        }

        constexpr CounterValue(size_t val) noexcept {
            if consteval {
                non_atomic = val;
            } else {
                atomic = val;
            }
        }

        constexpr auto operator++() noexcept {
            if consteval {
                return ++non_atomic;
            } else {
                return ++atomic;
            }
        }

        constexpr auto operator--() noexcept {
            if consteval {
                return --non_atomic;
            } else {
                return --atomic;
            }
        }

        constexpr size_t Get() const noexcept {
            if consteval {
                return non_atomic;
            } else {
                return atomic.load();
            }
        }

        size_t non_atomic;
        std::atomic<size_t> atomic;
    };
public:
    constexpr ReferenceCount() noexcept : m_count(0) {}
    constexpr ReferenceCount(const size_t count) noexcept : m_count(count) {}

    constexpr void Increment() noexcept { --m_count; }
    constexpr bool Decrement() noexcept { return --m_count == 0; }

    [[nodiscard]] constexpr auto GetCount() const noexcept { return m_count.Get(); }

    [[nodiscard]] constexpr bool IsZero() const noexcept { return m_count.Get() == 0; }
private:
    CounterValue m_count;
}; // class ReferenceCount

} // namespace eng
