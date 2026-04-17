#pragma once
#include <engine/eng_ReferenceCount.h>
#include "ObjectPoolTypes.h"

namespace eng::detail {

struct ControlBlock;

class IControlBlockDeallocator {
private:
    friend struct ControlBlock;
    virtual constexpr void Free(ControlBlock* p_blk) = 0;
}; // class IControlBlockDeallocator

struct ControlBlock {
    constexpr ControlBlock(IControlBlockDeallocator* man, void* p_val) noexcept
        : ref_cnt(),
          manager(man),
          p_val(p_val) {};
    ControlBlock(const ControlBlock& other) = delete;
    ControlBlock(ControlBlock&& other) = delete;

    template<typename T>
    [[nodiscard]] constexpr auto Get() const noexcept { return static_cast<T*>(p_val); }

    constexpr bool AddOne() noexcept {
        assert(ref_cnt.GetCount() > 0);
        ref_cnt.Increment();
    }

    constexpr void AddWeak() noexcept { weak_ref_cnt.Increment(); }

    constexpr bool ReleaseOne() {
        assert(ref_cnt.GetCount() > 0);
        ref_cnt.Decrement();
        return this->TryFree();
    }

    constexpr bool ReleaseWeak() {
        assert(weak_ref_cnt.GetCount() > 0);
        weak_ref_cnt.Decrement();
        return this->TryFree();
    }

    constexpr bool TryFree() {
        if (ref_cnt.IsZero() && weak_ref_cnt.IsZero()) {
            manager->Free(this); // we need to be careful to no operate on ourself after this
            return true;
        }
        return false;
    }

    ReferenceCount ref_cnt;
    ReferenceCount weak_ref_cnt;
    IControlBlockDeallocator* manager;
    void* p_val; // this can be eliminated if I abondon the constexpr dream
}; // struct ControlBlock

template<typename T>
struct ControlBlockImpl : public ControlBlock {
    template<typename... Args>
    constexpr ControlBlockImpl(IControlBlockDeallocator* man, Args&&... args) :
        ControlBlock(man, &val),
        val(std::forward<Args>(args)...) {}

    T val;
}; // struct ControlBlockImpl

} // namespace eng::detail
