#pragma once
#include <concepts>
#include <memory>
#include <type_traits>
#include <vector>

#include <engine/eng_Hash.h>
#include <engine/eng_ObjectManager.h>
#include <engine/eng_Rng.h>

#include <engine/shader/shdr_PointLight.h>

namespace eng {

class Object;
class Engine;

/**
 * Scene is a basic object container class.
 */
class Scene {
public:
    template<typename T>
    class ObjectCollection {
    public:
        /**
         * Add an named initialized object to the scene.
         * 
         * @param handle  Handle of the object to be added
         * @return Error code.
         */
        int Insert(std::string_view name, Handle<T> handle);

        /**
         * Add an unnamed initialized shape to the scene.
         */
        int Insert(Handle<T> handle);

        Handle<T> Get(std::string_view name);

        bool Contains(std::string_view name);

        int Remove(std::string_view name);
        int Remove(Handle<T> name);

        const auto& GetList() const { return m_list; }

        void Reserve(size_t num);

        constexpr bool GetUpdateFlag() const noexcept { return m_updated; }
        constexpr void ClearUpdateFlag() noexcept { m_updated = false; }
    private:
        int InsertImpl(std::string_view name, Handle<T> handle);

        int RemoveImpl(std::string_view name);
    private:
        std::unordered_map<std::string_view, Handle<T>, Hash<std::string_view>> m_map;
        std::vector<Handle<T>> m_list;
        bool m_updated;
    }; // class ObjectCollection
public:
    /**************************************************
     * Init functions.
     **************************************************/
    Scene();
    ~Scene();

    Handle<Object> CreateObject(std::string_view name);

    Handle<Object> FindObject(std::string_view name);

    void RemoveObject(std::string_view name);

    Engine* GetParent() const noexcept { return m_pParent; }

    // General CreateComponent impl
    template<typename T, typename... Args>
    constexpr auto CreateComponent(Args&&... args) {
        static constexpr auto id = typeid(T);
        auto managerIter = m_ComponentManagerMap.find(id);

        ComponentManager<T>* pMan;
        if (managerIter == m_ComponentManagerMap.end()) {
            auto tmp = std::make_unique<ComponentManager<T>>();
            pMan = tmp.get();
            m_ComponentManagerMap[id] = std::move(tmp);
        } else {
            pMan = managerIter->second.get();
        }

        return pMan->Create(std::forward<Args>(args)...);
    }
private:
    class IComponentManager {
    public:
        constexpr virtual ~IComponentManager() = default;

        constexpr virtual void EarlyUpdateAll() = 0;
        constexpr virtual void UpdateAll() = 0;
    };

    template<typename T>
    class ComponentManager {
    public:
        constexpr virtual void EarlyUpdateAll() override {
            m_Manager.InvokeOnAll(&T::EarlyUpdateImpl);
        }

        constexpr virtual void UpdateAll() override {
            m_Manager.InvokeOnAll(&T::UpdateImpl);
        }

        template<typename... Args>
        constexpr auto Create(Args&&... args) {
            return m_Manager.CreateObject(std::forward<Args>(args)...);
        }
    private:
        ObjectManager<T> m_Manager
    };
protected:
    Engine* m_pParent;
    ObjectManager<Object> m_Manager;
    std::unordered_map<std::string_view, Handle<Object>> m_ObjectMap;

    std::unordered_map<std::type_info, std::unique_ptr<IComponentManager>> m_ComponentManagerMap;

    bool m_objectsUpdated;
}; // class Scene

} // namespace eng
