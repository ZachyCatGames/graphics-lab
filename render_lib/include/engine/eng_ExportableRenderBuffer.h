#pragma once
#include <engine/eng_RenderBuffer.h>
#include <engine/framebuffer/fb_Framebuffer.h>

namespace eng {

class ExportableRenderBuffer : public RenderBuffer {
public:
    [[nodiscard]] virtual fb::Framebuffer ExportToFramebuffer() const = 0;
}; // class ExportableRenderBuffer

} // namespace eng
