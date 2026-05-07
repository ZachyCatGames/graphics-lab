#pragma once
#include <functional>
#include <vector>

namespace eng {

template<typename Interface, auto Func, typename InterfacePtrType = Interface*>
struct EventDispatcher {
public:
    template<typename... Args>
    void NotifyAll(Args&&... args) const {
        for (auto rec : receivers) {
            std::invoke(Func, rec, std::forward<Args>(args));
        }
    }

    std::vector<InterfacePtrType> receivers;
}; // class EventDispatcher

} // namespace eng
