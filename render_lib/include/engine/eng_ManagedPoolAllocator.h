#pragma once
#include <engine/detail/ObjectPoolImpl.h>

namespace eng {

template<typename T, typename AllocatorType = std::allocator<T>>
class ManagedPoolAllocator : public detail::PoolAllocatorImpl<T, ManagedPoolAllocator<T, AllocatorType>, true> {
private:
    using BaseType = detail::PoolAllocatorImpl<T, ManagedPoolAllocator<T, AllocatorType>, true>;
    friend class detail::PoolAllocatorImpl<T, ManagedPoolAllocator<T, AllocatorType>, true>;
public:
    void Initialize(const size_t initialObjectCount) {
        BaseType::Initialize(initialObjectCount);
    }
private:
    std::pair<std::byte*, size_t> RequestExpansion(size_t numBytes) {
        std::byte* pNewBytes = m_Allocator.allocate(numBytes);
        if (pNewBytes == nullptr) {
            return { nullptr, 0 };
        }
        return { pNewBytes, numBytes };
    }
private:
    using __AllocType = typename std::allocator_traits<AllocatorType>::rebind_alloc<std::byte>;

    __AllocType m_Allocator;
}; // class ManagedPoolAllocator

} // namespace eng
