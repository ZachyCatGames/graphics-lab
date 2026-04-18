#pragma once
#include <engine/eng_Handle.h>
#include <engine/detail/ControlBlock.h>
#include <engine/detail/eng_HandleConstructor.h>

namespace eng {

namespace detail {

class GlobalObjectDeleter : public IControlBlockDeallocator {
public:
    constexpr GlobalObjectDeleter() = default;

    virtual void Free(ControlBlock* pBlock) override {
        delete pBlock;
    }
}; // class GlobalObjectDeleter

constinit GlobalObjectDeleter g_MakeSharedDeleter{};

} // namespace detail

template<typename T, typename... Args>
Handle<T> MakeShared(Args&&... args) {
    auto pControlBlock = new detail::TypedControlBlock<T>(&detail::g_MakeSharedDeleter, std::forward<Args>(args)...);
    return detail::HandleConstructor::ConstructFromControlBlockPointer(pControlBlock);
}

} // namespace eng
