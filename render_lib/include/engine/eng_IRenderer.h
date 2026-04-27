#pragma once
#include <engine/framebuffer/fb_Framebuffer.h>
#include <engine/eng_RenderBuffer.h>
#include <engine/eng_Handle.h>
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
    virtual void Render(std::string_view cameraName, Handle<RenderBuffer> fb) = 0;
}; // class IRenderer

} // namespace eng
