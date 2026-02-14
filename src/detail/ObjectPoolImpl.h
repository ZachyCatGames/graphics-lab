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

    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<PoolEntry>;
public:
    constexpr ~ObjectPoolImplBase() {
        /* We can't really cleanly cleanup... just trash everything. */
        PoolEntry* p_ent = m_cur_pool_block;
        while (p_ent != nullptr) {
            auto p_prev = p_ent->hdr.p_prev;
            m_alloc.deallocate(p_ent, p_ent->hdr.n);
            p_ent = p_prev;
        }
    }

    constexpr bool IsInitialized() const { return m_cur_pool_block != nullptr; }
protected:
    constexpr ObjectPoolImplBase(const Allocator& alloc = {}) :
        m_alloc(alloc), m_cur_pool_block(nullptr), m_first_free(nullptr) {}

    constexpr ObjectPoolImplBase(Allocator&& alloc) :
        m_alloc(std::move(alloc)), m_cur_pool_block(nullptr), m_first_free(nullptr) {}

    constexpr void Initialize(const Allocator& alloc = {}) {
        assert(!this->IsInitialized());

        /* Set Allocator. */
        m_alloc = alloc;

        /* Allocate initial block. */
        this->AllocateNewBlock(Traits::InitialObjectCount);
    }

    constexpr void Initialize(Allocator&& alloc) {
        assert(!this->IsInitialized());

        /* Set Allocator. */
        m_alloc = std::move(alloc);

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

        /* Cast back to a pool entry. */
        //auto p_pool_ent = new (p_obj) PoolEntry(m_first_free);
        //auto p_pool_ent = static_cast<PoolEntry*>(static_cast<void*>(p_obj));

        /* Insert back into the free list. */
        p_pool_ent->next_free = m_first_free;
        m_first_free = p_pool_ent;
    }
private:
    [[nodiscard]] constexpr auto AllocateImpl() {
        /* Allocate a new block if needed. */
        if (m_cur_pool_block == nullptr) {
            this->AllocateNewBlock(Traits::InitialObjectCount);
            assert(m_cur_pool_block != nullptr);
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
        auto new_blk = m_alloc.allocate(n);

        /* First block is a pointer to the previous.*/
        std::construct_at(&new_blk[0], BlockHeader{ m_cur_pool_block, n});

        /* Initialize each block to point to the next block. */
        for (size_t i = 1; i < n; i++) {
            std::construct_at(&new_blk[i], &new_blk[i+1]);
        }

        /* Link the beginning and ending. */
        std::construct_at(&new_blk[n-1], m_first_free);
        m_first_free = &new_blk[1];

        /* Point the current pool block at the new block. */
        m_cur_pool_block = new_blk;
    }
private:
    allocator_type m_alloc;
    PoolEntry* m_cur_pool_block;
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
    constexpr auto GetDerived() { return static_cast<ManagerType*>(this); }
    constexpr auto GetDerived() const { return static_cast<const ManagerType*>(this); }
}; // class ObjectPoolImpl

} // namespace eng
