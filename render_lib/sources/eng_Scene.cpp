#include <engine/eng_Scene.h>

namespace eng {

namespace {

constexpr std::string_view AnonymousPrefix = "__A_";

constexpr bool IsAnonymousName(std::string_view name) {
    return name.starts_with(AnonymousPrefix);
}

template<typename T>
std::string CreateAnonymousName(const Handle<T>& obj) {
    return std::string(AnonymousPrefix) + std::format("{}", std::bit_cast<intptr_t>(obj.Get()));
}

} // namespace


Scene::Scene() :
    m_objectsUpdated(false) {}


template<typename T>
int Scene::ObjectCollection<T>::InsertImpl(std::string_view name, Handle<T> handle) {
    /* Does a shape with this name already exist? */
    if(this->Contains(name))
        return -1;

    /* Mark the BVH as requiring an update. */
    m_updated = true;

    m_list.push_back(handle);
    m_map.insert_or_assign(name, handle);

    return 0;
}

template<typename T>
int Scene::ObjectCollection<T>::Insert(std::string_view name, Handle<T> handle) {
    if(IsAnonymousName(name))
        return -2;
    
    return this->InsertImpl(name, handle);
}

template<typename T>
int Scene::ObjectCollection<T>::Insert(Handle<T> handle) {
    auto name = CreateAnonymousName(handle);
    return this->InsertImpl(name, handle);
}

template<typename T>
Handle<T> Scene::ObjectCollection<T>::Get(std::string_view name) {
    if(!this->Contains(name)) {
        return nullptr;
    }

    return m_map[name];
}

template<typename T>
bool Scene::ObjectCollection<T>::Contains(std::string_view name) { return m_map.contains(name); }

template<typename T>
int Scene::ObjectCollection<T>::RemoveImpl(std::string_view name) {
    /* Mark the BVH as requiring an update. */
    m_updated = true;

    /* Find the shape. */
    auto obj = this->Get(name);
    if(!obj)
        return -1;

    /* Find where the shape is in the shape list. */
    auto it = std::ranges::find(m_list, obj);
    assert(it != m_list.end());

    /* Remove it from the shapes list. */
    m_list.erase(it);

    /* Remove is from the attribs map. */
    m_map.erase(name);

    return 0;
}

template<typename T>
int Scene::ObjectCollection<T>::Remove(std::string_view name) {
    if(!IsAnonymousName(name))
        return -2;

    return this->RemoveImpl(name);
}

template<typename T>
int Scene::ObjectCollection<T>::Remove(Handle<T> handle) {
    auto name = CreateAnonymousName(handle);
    return this->RemoveImpl(name);
}

template<typename T>
void Scene::ObjectCollection<T>::Reserve(size_t num) { m_list.reserve(num); }

template class Scene::ObjectCollection<IShape>;
template class Scene::ObjectCollection<IShader>;
template class Scene::ObjectCollection<ICamera>;

shdr::PointLight& Scene::EmplacePointLight(const Vector3DF& pos, const Vector3DF& intensity) {
    return m_lights.emplace_back(pos, intensity);
}

shdr::PointLight& Scene::InsertPointLight(const shdr::PointLight& light) {
    m_lights.push_back(light);
    return m_lights.back();
}

void Scene::ReservePointLights(size_t count) { m_lights.reserve(count); }

} // namespace eng
