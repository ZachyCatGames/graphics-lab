#include "ObjectManager.h"

namespace eng {

/**
 * Object base class.
 * 
 * Provides an allocation function for the derived class via CRTP.
 */
template<typename T, typename AllocatorType = std::allocator<T>>
class ObjectBase {
public:
    constexpr ObjectBase() = default;

    template<typename... Args>
    [[nodiscard]] static constexpr auto Create(Args&&... args);
}; // class ObjectBase

template<typename T, typename AllocatorType>
template<typename... Args>
[[nodiscard]] constexpr auto ObjectBase<T, AllocatorType>::Create(Args&&... args) {
    return ObjectManager<T, AllocatorType>::Get()->CreateObject(std::forward<Args>(args)...);
}

} // namespace eng
