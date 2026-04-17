#pragma once
#include <engine/framebuffer/fb_Framebuffer.h>
#include <engine/eng_Material.h>
#include <engine/eng_Shape.h>
#include <engine/eng_Translation.h>
#include <string_view>
#include <vector>

namespace eng {

class Renderer {
public:
    virtual ~Renderer();

    virtual void AddRenderObject(Handle<Shape> mesh, Handle<Material> mat, Handle<Translation> pos);

    virtual void ClearRenderObjects();

    virtual void ReserveRenderObjects(size_t num);

    /**
     * Perform pre-rendering setup.
     */
    virtual void PreRender() = 0;

    /**
     * Render the held scene to a framebuffer through a specified
     * camera.
     * 
     * @param cameraName  Name of the camera to use for rendering.
     * @param p_fb  Framebuffer to render out to.
     */
    virtual void Render(std::string_view cameraName, fb::Framebuffer* p_fb) = 0;
protected:
    std::vector<RenderObject> m_RenderObjects;
}; // class Renderer

} // namespace eng
