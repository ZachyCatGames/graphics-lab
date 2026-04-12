#pragma once
#include <engine/framebuffer/fb_Framebuffer.h>
#include <string_view>

namespace eng {

class IRenderer {
public:
    virtual ~IRenderer() = default;

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
}; // class IRenderer

} // namespace eng
