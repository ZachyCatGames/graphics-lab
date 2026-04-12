#pragma once
#include <cassert>
#include <cstring>
#include <memory>
#include <new>
#include "ObjectPoolTypes.h"

namespace eng::detail {

template<typename T>
struct PoolTraits {
    static constexpr size_t InitialObjectCount = 100;
};

template<typename T, typename Allocator = std::allocator<T>>
class ObjectPoolImplBase {
public:
    /* Public aliases. */
    using value_type = std::remove_reference_t<T>;
    using reference_type = value_type&;
    using pointer_type = value_type*;
private:
    using Traits = PoolTraits<T>;

    using PoolEntry   = PoolEntry<value_type>;
    using BlockHeader = BlockHeader<value_type>;

    using alloc_type = typename std::allocator_traits<Allocator>::template rebind_alloc<std::byte>;
    
    static constexpr size_t ObjectSize = std::max(alignof(value_type), sizeof(value_type));
public:
    constexpr ~ObjectPoolImplBase() {
        /* 
         * NOTE: This will result in memory being leaked in consteval
         * if we don't free objects beforethe allocator is destroyed.
         * My solution is "don't skill issue." "*/
        if consteval {
            return;
        }

        /* We can't really cleanly cleanup... just trash everything. */
        auto p_ent = m_cur_pool_block;
        while (p_ent != nullptr) {
            auto p_prev = p_ent->p_prev;
            m_allocator.deallocate(reinterpret_cast<std::byte*>(p_ent), p_ent->n);
            p_ent = p_prev;
        }
    }

    constexpr bool IsInitialized() const noexcept {
        if consteval {
            return true;
        }

        return m_cur_pool_block != nullptr;
    }
protected:
    constexpr ObjectPoolImplBase(const Allocator& alloc = {})
        : m_allocator(alloc),
          m_cur_pool_block(nullptr),
          m_first_free(nullptr) {}

    constexpr void Initialize(const Allocator& alloc = {}) {
        /* NOTE: in consteval this is basically just a wrapper around ::new. */
        if consteval {
            return;
        }

        assert(!this->IsInitialized());

        /* Set Allocator. */
        m_allocator = alloc;

        /* Setup initial allocation size. */
        m_allocationSize = Traits::InitialObjectCount;

        /* Allocate initial block. */
        this->AllocateNewBlock(m_allocationSize);
    }

    template<typename... Args>
    [[nodiscard]] constexpr auto CreateObjectImpl(Args&&... args) {
        if consteval {
            return new value_type(std::forward<Args>(args)...);
        }

        /* Allocate an object. */
        pointer_type obj = this->AllocateImpl();

        /* Initialize it. */
        return std::construct_at(obj, std::forward<Args>(args)...);
    }

    void FreeImpl(pointer_type p_obj) {
        if consteval {
            delete p_obj;
        }

        /* Destroy the contained object. */
        std::destroy_at(p_obj);

        /* Re-construct it as a PoolEntry. */
        auto poolEntry = ::new(p_obj) PoolEntry(m_first_free);

        /* Insert back into the free list. */
        m_first_free = poolEntry;
    }
private:

    [[nodiscard]] pointer_type AllocateImpl() {
        /* Allocate a new block if needed. */
        if (m_first_free == nullptr) {
            assert(m_cur_pool_block != nullptr);
            /* Double the allocation size. */
            m_allocationSize *= 2;

            this->AllocateNewBlock(m_allocationSize);
            assert(m_first_free != nullptr);
        }

        /* Get first free object. */
        PoolEntry* cur = m_first_free;

        /* Update first free object to next object. */
        m_first_free = cur->next_free;

        /* Destroy next iterator / pointer. */
        std::destroy_at(&cur->next_free);

        return &cur->obj;
    }

    void AllocateNewBlock(size_t n) {
        assert(n >= 2);
        size_t allocSize = sizeof(BlockHeader) - 1 + n * ObjectSize;
        std::byte* pNewMem = m_allocator.allocate(allocSize);

        /* Initialize the block header, link it to the current block. */
        auto pBlkHeader = ::new(pNewMem) BlockHeader(m_cur_pool_block, allocSize);

        /* Pointer to the first object. */
        PoolEntry* pObj = pBlkHeader->b;

        /* Initialize each block to point to the next block. */
        for (size_t i = 0; i < n - 1; i++) {
            auto pNext = pObj + 1;
            std::construct_at(pObj, pNext);
            pObj = pNext;
        }

        /* Link the beginning and ending. */
        std::construct_at(pObj, nullptr);
        m_first_free = pBlkHeader->b;

        /* Link this block as the current block. */
        m_cur_pool_block = pBlkHeader;
    }
private:
    alloc_type m_allocator;
    BlockHeader* m_cur_pool_block;
    PoolEntry* m_first_free;
    size_t m_allocationSize;
}; // class ObjectPool


template<typename T, typename ManagerType, typename AllocatorType>
class ObjectPoolImpl : public ObjectPoolImplBase<T, AllocatorType> {
public:
    using value_type     = ObjectPoolImplBase<T, AllocatorType>::value_type;
    using reference_type = ObjectPoolImplBase<T, AllocatorType>::reference_type;
    using pointer_type   = ObjectPoolImplBase<T, AllocatorType>::pointer_type;
protected:
    template<typename... Args>
    [[nodiscard]] constexpr auto CreateObject(Args&&... args) {
        /* Allocate and initialize the object. */
        auto obj = this->CreateObjectImpl(std::forward<Args>(args)...);

        /* Call NotifyAllocate on manager. */
        if constexpr (requires { GetDerived()->NotifyAllocate(&obj); })
            GetDerived()->NotifyAllocate(&obj);

        return obj;
    }

    constexpr void Free(pointer_type p_obj) {
        /* Call NotifyFree on manager. */
        if constexpr (requires { GetDerived()->NotifyFree(&p_obj); })
            GetDerived()->NotifyFree(p_obj);

        /* Call FreeImpl. */
        this->FreeImpl(p_obj);
    }
protected:
    using ObjectPoolImplBase<T, AllocatorType>::ObjectPoolImplBase;
private:
    constexpr auto GetDerived() noexcept { return static_cast<ManagerType*>(this); }
    constexpr auto GetDerived() const noexcept { return static_cast<const ManagerType*>(this); }
}; // class ObjectPoolImpl

} // namespace eng
