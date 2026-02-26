#pragma once
#include <cassert>
#include <cstring>
#include <memory>
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

    using block_alloc_type  = typename std::allocator_traits<Allocator>::template rebind_alloc<PoolEntry>;
    using header_alloc_type = typename std::allocator_traits<Allocator>::template rebind_alloc<BlockHeader>;
public:
    constexpr ~ObjectPoolImplBase() {
        /* We can't really cleanly cleanup... just trash everything. */
        auto p_ent = m_cur_pool_block;
        while (p_ent != nullptr) {
            auto p_prev = p_ent->p_prev;
            m_block_alloc.deallocate(p_ent->p_block, p_ent->n);
            m_header_alloc.deallocate(p_ent, 1);
            p_ent = p_prev;
        }
    }

    constexpr bool IsInitialized() const noexcept { return m_cur_pool_block != nullptr; }
protected:
    constexpr ObjectPoolImplBase(const Allocator& alloc = {})
        : m_header_alloc(alloc),
          m_block_alloc(alloc),
          m_cur_pool_block(nullptr),
          m_first_free(nullptr) {}

    constexpr void Initialize(const Allocator& alloc = {}) {
        assert(!this->IsInitialized());

        /* Set Allocator. */
        m_header_alloc = alloc;
        m_block_alloc  = alloc;

        /* Allocate initial block. */
        this->AllocateNewBlock(Traits::InitialObjectCount);
    }

    template<typename... Args>
    [[nodiscard]] constexpr auto CreateObjectImpl(Args&&... args) {
        /* Allocate an object. */
        auto obj = this->AllocateImpl();

        /* Initialize it. */
        std::construct_at(obj.Get(), std::forward<Args>(args)...);

        return obj;
    }

    constexpr void FreeImpl(PoolObjectHolder<value_type> p_obj) {
        /* Destroy the contained object. */
        auto p_pool_ent = p_obj.m_entry;
        std::destroy_at(&p_pool_ent->obj);

        /* Insert back into the free list. */
        p_pool_ent->next_free = m_first_free;
        m_first_free = p_pool_ent;
    }
private:
    // Only used for contexpr allocations.
    struct AllocationBlock {
        BlockHeader hdr;
        PoolEntry entries[Traits::InitialObjectCount];
    }; // struct AllocationBlock

    [[nodiscard]] constexpr auto AllocateImpl() {
        /* Allocate a new block if needed. */
        if (m_first_free == nullptr) {
            assert(m_cur_pool_block != nullptr);
            this->AllocateNewBlock(m_cur_pool_block->n * 2);
            assert(m_first_free != nullptr);
        }

        /* Get first free object. */
        auto cur = m_first_free;

        /* Update first free object to next object. */
        m_first_free = cur->next_free;

        /* Destroy next iterator / pointer. */
        std::destroy_at(&cur->next_free);

        return PoolObjectHolder(cur);
    }

    constexpr void AllocateNewBlock(size_t n) {
        assert(n >= 2);
        auto new_blk = m_block_alloc.allocate(n);

        /* Initialize each block to point to the next block. */
        for (size_t i = 0; i < n; i++) {
            std::construct_at(&new_blk[i], &new_blk[i+1]);
        }

        /* Link the beginning and ending. */
        std::construct_at(&new_blk[n-1], m_first_free);
        m_first_free = &new_blk[1];

        /* Terminate end. */
        std::construct_at(&new_blk[n-1], nullptr);

        /* Point the current pool block at the new block. */
        m_first_free = new_blk;

        /* Allocate block list header. */
        auto p_hdr = m_header_alloc.allocate(1);

        /* Link it in. */
        std::construct_at(p_hdr, m_cur_pool_block, m_first_free, n);
        m_cur_pool_block = p_hdr;
    }
private:
    block_alloc_type m_block_alloc;
    header_alloc_type m_header_alloc;
    BlockHeader* m_cur_pool_block;
    PoolEntry* m_first_free;
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

    constexpr void Free(PoolObjectHolder<value_type> p_obj) {
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
