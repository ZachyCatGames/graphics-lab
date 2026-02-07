#include "NativeWindowWriter.plat-nx.h"

namespace eng::plat::nx {

NativeWindowWriter::NativeWindowWriter(::Framebuffer* fb) : m_framebuffer(fb) {
    
}

NativeWindowWriter::~NativeWindowWriter() {

}

void NativeWindowWriter::WriteFramebufferPixelImpl(const Framebuffer& fb, int x, int y) {
    const auto fb_pos = y * m_stride / sizeof(u32) + x;

    /* Get the current pixel's color. */
    const auto color = fb.GetPixelColor(x, y).clamp(1.0) * 255.0;

    /* Convert to an RGBA8 word and write to our fb. */
    m_fb_ptr[fb_pos] = RGBA8_MAXALPHA(static_cast<u8>(color.x()), static_cast<u8>(color.y()), static_cast<u8>(color.z()));
}

void NativeWindowWriter::NotifyWriteBegin(int width, int height) {
    /* Get a ptr into our framebuffer. */
    m_fb_ptr = static_cast<u32*>(framebufferBegin(m_framebuffer, &m_stride));
}

void NativeWindowWriter::NotifyAllPixelsWritten(const Framebuffer&) {
    ::framebufferEnd(m_framebuffer);
}

} // namespace eng::plat::nx
