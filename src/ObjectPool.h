#pragma once
#include <memory>

namespace eng {

namespace detail {

class ObjectManagerStub {};

} // namespace detail

template<typename T, typename AllocatorType = std::allocator<T>>
class ObjectPool : public detail::ObjectPoolImpl<T, detail::ObjectManagerStub, AllocatorType> {
public:
    using value_type = detail::ObjectPoolImpl<T, detail::ObjectManagerStub, AllocatorType>::value_type;
    using reference_type = detail::ObjectPoolImpl<T, detail::ObjectManagerStub, AllocatorType>::reference_type;
    using pointer_type = detail::ObjectPoolImpl<T, detail::ObjectManagerStub, AllocatorType>::pointer_type;

    using detail::ObjectPoolImpl<T, detail::ObjectManagerStub, AllocatorType>::ObjectPoolImpl;

    //constexpr ObjectPool() = default;
    //constexpr ObjectPool(const AllocatorType& alloc) : detail::ObjectPoolImpl<T, detail::ObjectManagerStub, AllocatorType>(alloc) {}
    //constexpr ObjectPool(AllocatorType&& alloc) : detail::ObjectPoolImpl<T, detail::ObjectManagerStub, AllocatorType>(std::move(alloc)) {}
}; // class ObjectPool

} // namespace eng
