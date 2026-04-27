#include "engine/eng_ExportableRenderBuffer.h"
#include "engine/framebuffer/fb_Framebuffer.h"
#include <engine/raytracer/eng_ExportableRenderBuffer.h>

namespace eng::rt {

ExportableRenderBuffer::ExportableRenderBuffer(size_t width, size_t height) : 
    eng::ExportableRenderBuffer(width, height), m_framebuffer(width, height) {}

void ExportableRenderBuffer::SetPixelColor(size_t x, size_t y, Vector3DF color) {
    m_framebuffer.SetPixelColor(x, y, color);
}

fb::Framebuffer ExportableRenderBuffer::ExportToFramebuffer() const { return m_framebuffer; }

} // namespace eng::rt
