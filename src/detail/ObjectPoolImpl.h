#pragma once
#include <cassert>
#include <memory>

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

    union PoolEntry;

    struct BlockHeader {
        PoolEntry* p_prev;
        size_t n;
    };

    union PoolEntry {
        BlockHeader hdr;
        T obj;
        PoolEntry* next_free;
    };

    using allocator_type = typename std::allocator_traits<Allocator>::rebind_alloc<PoolEntry>;
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
        std::construct_at(obj, std::forward<Args>(args)...);

        return obj;
    }

    constexpr void FreeImpl(pointer_type p_obj) {
        /* Destroy both the ref count and object. */
        std::destroy_at(p_obj);

        /* Insert back into the free list. */
        /* Note: p_obj is actually to our union, specifically the control block object. */
        /* Note2: This... might not be constexpr approved, but _shrug_. */
        auto p_it = new (p_obj) PoolEntry*(m_first_free);
        m_first_free = *p_it;
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

        return &cur->obj;
    }

    constexpr void AllocateNewBlock(size_t n) {
        assert(n >= 2);
        auto new_blk = m_alloc.allocate(n);

        /* First block is a pointer to the previous.*/
        auto& hdr  = new_blk[0].hdr;
        hdr.p_prev = m_cur_pool_block;
        hdr.n      = n;

        /* Initialize each block to point to the next block. */
        for (size_t i = 1; i < n; i++)
            new_blk[i].next_free = &new_blk[i+1];

        /* Link the beginning and ending. */
        new_blk[n-1].next_free = m_first_free;
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
        if constexpr (requires { GetDerived()->NotifyAllocate(&obj->val); })
            GetDerived()->NotifyAllocate(&obj);

        return obj;
    }

    constexpr void Free(pointer_type p_obj) {
        /* Call NotifyFree on manager. */
        if constexpr (requires { GetDerived()->NotifyFree(&p_obj->val); })
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
