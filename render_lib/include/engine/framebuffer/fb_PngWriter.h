#pragma once
#include <engine/framebuffer/fb_FramebufferWriterBase.h>

#include <engine/png++/png.hpp>

namespace eng::fb {

class PngWriter : public FramebufferWriterBase<PngWriter> {
public:
    PngWriter(std::string_view path);
private:
    friend class FramebufferWriterBase<PngWriter>;
    void WriteFramebufferPixelImpl(const Framebuffer& fb, int x, int y);

    void NotifyWriteBegin(int width, int height);
    void NotifyAllPixelsWritten(const Framebuffer&);
private:
    std::string m_path;
    png::image<png::rgb_pixel> m_img;
    int m_width, m_height;
}; // class PngWriter

} // namespace eng::fb
