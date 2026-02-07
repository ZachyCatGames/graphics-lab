#pragma once
#include "FramebufferWriterBase.h"

#include "../png++/png.hpp"

namespace eng {

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
}; // class PngWriter

} // namespace eng
