#pragma once
#include <string_view
#include <unordered_map>

namespace eng {

template<typename Derived, typename ValueType>
class NamedResourceManagerBase {
public:
    auto GetResource(std::string_view name) {
        try {
            return m_Map.at(name);
        }
        catch (std::out_of_range& except) {
            auto res = static_cast<Derived*>(this)->CreateResource(name);
            m_Map[name] = res;
            return res;
        }
    }

    void RemoveResource(std::string_view name) {
        m_Map.erase(name)
    }
protected:
    std::unordered_map<std::string_view, ValueType> m_Map;
}; // class NamedResourceManagerBase

} // namespace eng
