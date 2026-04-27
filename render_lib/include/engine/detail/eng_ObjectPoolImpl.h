#pragma once
#include <cassert>
#include <cstring>
#include <memory>
#include <new>
#include "eng_ObjectPoolTypes.h"

namespace eng::detail {

template<typename T>
struct PoolTraits {
    static constexpr size_t InitialObjectCount = 100;
};

/*
template<typename T>
concept SupportsRequestExpansion = requires(T&& inst, size_t n) {
    { inst.RequestExpansion(n) };
};*/

template<typename T, typename Derived, bool Extendable = false>
class PoolAllocatorImpl {
public:
    /* Public aliases. */
    using value_type = std::remove_reference_t<T>;
    using reference_type = value_type&;
    using pointer_type = value_type*;
private:
    using Traits = PoolTraits<T>;

    using PoolEntryT   = PoolEntry<value_type>;
    using BlockHeaderT = BlockHeader<value_type>;

    static constexpr size_t MinimumAllocationRequirement = sizeof(BlockHeaderT);
    static constexpr size_t ObjectSize = sizeof(value_type);
public:
    constexpr PoolAllocatorImpl()
        : m_cur_pool_block(nullptr),
          m_first_free(nullptr),
          m_allocationSize(0) {}

    constexpr ~PoolAllocatorImpl() = default;

    constexpr bool IsInitialized() const noexcept {
        return m_cur_pool_block != nullptr;
    }

    [[nodiscard]] pointer_type Allocate() {
        /* Allocate a new block if needed. */
        if (m_first_free == nullptr) {
            if constexpr (Extendable) {
                this->ExpandPoolImpl();
            } else {
                return nullptr;
            }
        }

        /* Get first free object. */
        PoolEntryT* cur = m_first_free;

        /* Update first free object to next object. */
        m_first_free = cur->next_free;

        /* Destroy next iterator / pointer. */
        std::destroy_at(&cur->next_free);

        return &cur->obj;
    }

    void Free(pointer_type p_obj) {
        /* Re-construct it as a PoolEntryT. */
        auto poolEntry = ::new(p_obj) PoolEntryT(m_first_free);

        /* Insert back into the free list. */
        m_first_free = poolEntry;
    }

    template<typename... Args>
    [[nodiscard]] pointer_type AllocateAndConstruct(Args&&... args) {
        // NOTE: this kinda breaks if T throws an exception, do we want to deal with that?
        return std::construct_at(this->Allocate(), std::forward<Args>(args)...);
    }

    void DestroyAndFree(pointer_type pObject) {
        std::destroy_at(pObject);
        this->Free(pObject);
    }
protected:
    static constexpr size_t CalculateBlockCount(size_t sizeBytes) {
        return (sizeBytes - sizeof(BlockHeaderT)) / PoolEntryT::ObjectSize;
    }

    void Initialize(std::byte* pMemory, size_t sizeBytes) {
        assert(!this->IsInitialized());

        /* Setup initial allocation size. */
        m_allocationSize = this->CalculateBlockCount(sizeBytes);

        /* Allocate initial block. */
        this->PrepareNewBlock(pMemory, sizeBytes);
    }
    
    void Initialize(size_t initialObjectCount) requires Extendable {
        m_allocationSize = initialObjectCount;

        this->ExpandPoolImpl();
    }

    void ExpandPoolImpl() requires Extendable {
        auto [pNewMem, newMemorySize] = GetDerived()->RequestExpansion(sizeof(BlockHeaderT) + m_allocationSize * ObjectSize);
        this->PrepareNewBlock(pNewMem, newMemorySize);
        assert(m_first_free != nullptr);

        /* Double the allocation size. */
        m_allocationSize *= 2;
    }
private:

    [[nodiscard]] Derived* GetDerived() { return static_cast<Derived*>(this); }

    void PrepareNewBlock(std::byte* pNewMem, size_t newMemorySize) requires Extendable {
        /* Initialize the block header, link it to the current block. */
        auto pBlkHeader = ::new(pNewMem) BlockHeaderT(m_cur_pool_block, newMemorySize);

        /* Pointer to the first object. */
        PoolEntryT* pObj = reinterpret_cast<PoolEntryT*>(pBlkHeader->b);

        /* Initialize each block to point to the next block. */
        size_t n = this->CalculateBlockCount(newMemorySize);
        for (size_t i = 0; i < n - 1; i++) {
            auto pNext = pObj + 1;
            std::construct_at(pObj, pNext);
            pObj = pNext;
        }

        /* Link the beginning and ending. */
        std::construct_at(pObj, nullptr);
        m_first_free = reinterpret_cast<PoolEntryT*>(pBlkHeader->b);

        /* Link this block as the current block. */
        m_cur_pool_block = pBlkHeader;
    }
private:
    BlockHeaderT* m_cur_pool_block;
    PoolEntryT* m_first_free;
    size_t m_allocationSize;
}; // class ObjectPool

} // namespace eng
