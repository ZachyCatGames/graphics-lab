#pragma once
#include <engine/eng_Scene.h>
#include <engine/framebuffer/fb_Framebuffer.h>

#include <concepts>
#include <type_traits>
#include <unordered_map>

namespace eng {

class RendererBase {
public:
    RendererBase()
        : m_p_scene(nullptr) {}

    RendererBase(Scene* p_scene, size_t samples_per_pixel, bool random_samples) {
        this->Initialize(p_scene, samples_per_pixel, random_samples);
    }

    virtual ~RendererBase();

    void Initialize(Scene* p_scene, size_t samples_per_pixel, bool random_samples);

    void Finalize();

    [[nodiscard]] bool IsInitialized() const noexcept { return m_p_scene != nullptr; }

    /**
     * Initializes a camera of type T with the provided args
     * and adds it to the camera list. The ID of the newly
     * added camera is returned.
     * 
     * @param args  Arguments forwarded to the T to be constructed.
     * @return ID of the newly constructed camera.
     */
    template<std::derived_from<ICamera> T, typename... Args>
    int EmplaceCamera(Args&&... args) {
        auto handle = T::Create(std::forward<Args>(args)...);
        return this->InsertCamera(handle);
    }

    /**
     * Add an initialized camera to the camera list.
     * The ID of the newly added camera is returned.
     * 
     * @param camera  Handle of camera to be added.
     * @return ID of the newly added camera.
     */
    int InsertCamera(Handle<ICamera> camera);

    /**
     * Returns whether a given camera ID is valid / available.
     * 
     * @param id  Camera ID to check.
     * @return  True if the id is valid, otherwise false.
     */
    [[nodiscard]] bool IsCameraIdValid(int id);

    /**
     * Remove a camera with a given ID.
     * 
     * @param id  ID of the camera to be removed.
     */
    void RemoveCamera(int id);

    /**
     * Render the held scene to a framebuffer through a specified
     * camera.
     * 
     * @param camera_id  ID of the camera to use for rendering.
     * @param p_fb  Framebuffer to render out to.
     */
    virtual void Render(int camera_id, fb::Framebuffer* p_fb) = 0;
protected:
    Vector3DF GetPixelColor(Handle<ICamera> p_cam, int x, int y);
protected:
    Scene* m_p_scene;
    
    std::unordered_map<int, Handle<ICamera>> m_cameras;
    int m_current_camera_id;

    size_t m_samples_per_pixel;
    bool m_randomize_samples;
}; // class RendererBase

} // namespace eng
