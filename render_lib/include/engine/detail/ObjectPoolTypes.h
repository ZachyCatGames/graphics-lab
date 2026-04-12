#pragma once

namespace eng::detail {

template<typename T>
union PoolEntry;

template<typename T>
union PoolEntry {
    explicit constexpr PoolEntry(PoolEntry<T>* next_free) noexcept
        : next_free(next_free) {}

    explicit constexpr PoolEntry(std::nullptr_t) noexcept
        : next_free(nullptr) {}

    static constexpr size_t ObjectSize = std::max(alignof(T), sizeof(T));


    T obj;
    PoolEntry<T>* next_free;
    std::byte bytes[ObjectSize]; // this is for forcing aligned strides in some loops in the pool allocator
};

template<typename T>
struct BlockHeader {
    constexpr BlockHeader(BlockHeader<T>* prev, size_t n) noexcept
        : p_prev(prev),
          n(n) {}

    BlockHeader<T>* p_prev;
    size_t n;

    static constexpr size_t ObjectSize = std::max(alignof(T), sizeof(T));

    alignas(alignof(T)) PoolEntry<T> b[0]; // used as beginning location of objects.
};

} // namespace eng::detail
