#pragma once
#include <engine/eng_Handle.h>
#include <engine/detail/ControlBlock.h>
#include <engine/detail/eng_HandleConstructor.h>
#include <engine/detail/eng_ObjectManager.h>

namespace eng {

namespace detail {

class GlobalObjectDeleter : public IControlBlockDeallocator {
public:
    constexpr GlobalObjectDeleter() = default;

    virtual void Free(ControlBlock* pBlock) override {
        delete pBlock;
    }
}; // class GlobalObjectDeleter

constinit inline GlobalObjectDeleter g_MakeSharedDeleter{};

} // namespace detail

template<typename T, typename... Args>
Handle<T> MakeShared(Args&&... args) {
    auto pControlBlock = new detail::TypedControlBlock<T>(&detail::g_MakeSharedDeleter, std::forward<Args>(args)...);
    return detail::HandleConstructor<T>::ConstructFromControlBlockPointer(pControlBlock);
}

template<typename T, typename... Args>
Handle<T> MakeSharedPooled(Args&&... args) {
    return ObjectManager<T>::Get()->CreateObject(std::forward<Args>(args)...);
}

} // namespace eng
