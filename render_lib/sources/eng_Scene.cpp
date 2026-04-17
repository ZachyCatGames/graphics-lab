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

Scene::~Scene() = default;

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

Handle<Object> Scene::CreateObject(std::string_view name) {
    /* Does this name already exist? */
    auto cur = m_ObjectMap.find(name);
    if (cur != m_ObjectMap.end())
        return cur->second;

    auto newObj = m_Manager.CreateObject();
    m_ObjectMap[name] = newObj;
    return newObj;
}

Handle<Object> Scene::FindObject(std::string_view name) {
    auto iter = m_ObjectMap.find(name);
    if (iter == m_ObjectMap.end())
        return nullptr;

    return iter->second;
}

void Scene::RemoveObject(std::string_view name) {
    m_ObjectMap.erase(name);
}

} // namespace eng
