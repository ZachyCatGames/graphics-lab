#pragma once
#include <concepts>
#include <memory>
#include <type_traits>
#include <vector>

#include <engine/eng_Hash.h>
#include <engine/eng_ICamera.h>
#include <engine/eng_Interval.h>
#include <engine/eng_IShader.h>
#include <engine/eng_IShape.h>
#include <engine/eng_ObjectManager.h>
#include <engine/eng_Rng.h>

#include <engine/shader/shdr_PointLight.h>

namespace eng {

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

    /**************************************************
     * Shape functions.
     **************************************************/
    ObjectCollection<IShape> shapes;

    /**************************************************
     * Shader functions.
     **************************************************/
    ObjectCollection<IShader> shaders;

    /**************************************************
     * Camera functions.
     **************************************************/
    ObjectCollection<ICamera> cameras;

    /**************************************************
     * Point light functions.
     **************************************************/
    shdr::PointLight& EmplacePointLight(const Vector3DF& pos, const Vector3DF& intensity);

    shdr::PointLight& InsertPointLight(const shdr::PointLight& light);
    //void InsertPointLight(shdr::PointLight&& light);

    const std::vector<shdr::PointLight>& GetPointLights() const noexcept { return m_lights; }

    /** 
     * Reserve / preallocate point light objects.
     * 
     * @param count  Number of point lights to preallocate.
     */
    void ReservePointLights(size_t count);

    /**
     * Reserve / preallocate shape handles.
     * 
     * This can be used when the number of shapes being used is known ahead
     * of time to reduce the number of allocations / copies required from
     * vector resizes.
     * 
     * @param count  Number of shape handles to preallocate.
     */
protected:
    std::vector<shdr::PointLight> m_lights;

    bool m_objectsUpdated;
}; // class Scene

} // namespace eng
