#include "Framebuffer.h"

#include "png++/png.hpp"

void Framebuffer::ExportToPng(std::string_view path) {
	png::image<png::rgb_pixel> img(m_width, m_height);

	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			const auto clamped = m_fb[XyToIndex(x, y)].clamp(1.0);
			img[y][x] = png::rgb_pixel(clamped.x() * 255, clamped.y() * 255, clamped.z() * 255);
		}
	}

	img.write(path.data());
}
