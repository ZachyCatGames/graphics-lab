#include "PngWriter.h"

#include "../png++/png.hpp"

namespace eng {

PngWriter::PngWriter(std::string_view path) : m_path(path), m_img() {}

void PngWriter::WriteFramebufferPixelImpl(const Framebuffer& fb, int x, int y) {
    const auto color           = fb.GetPixelColor(x, y).clamp(1.0);

    /* Write the target pixel. */
    m_img[m_height - y - 1][x] = png::rgb_pixel(color.x() * 255, color.y() * 255, color.z() * 255);
}

void PngWriter::NotifyWriteBegin(int width, int height) {
    m_width  = width;
    m_height = height;
    m_img    = png::image<png::rgb_pixel>(width, height);
}

void PngWriter::NotifyAllPixelsWritten(const Framebuffer&) {
	m_img.write(m_path.c_str());
}

} // namespace eng
