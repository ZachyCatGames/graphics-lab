#include "engine/eng_ExportableRenderBuffer.h"
#include "engine/framebuffer/fb_Framebuffer.h"
#include <engine/raytracer/eng_ExportableRenderBuffer.h>

namespace eng::rt {

ExportableRenderBuffer::ExportableRenderBuffer(size_t width, size_t height) : 
    m_framebuffer(width, height) {}

void ExportableRenderBuffer::SetPixelColor(size_t x, size_t y, Vector3DF color) {
    m_framebuffer.SetPixelColor(x, y, color);
}

fb::Framebuffer ExportableRenderBuffer::ExportToFramebuffer() const { return m_framebuffer; }

size_t ExportableRenderBuffer::GetWidth() const { return m_framebuffer.GetWidth(); }
size_t ExportableRenderBuffer::GetHeight() const { return m_framebuffer.GetHeight(); }

} // namespace eng::rt
