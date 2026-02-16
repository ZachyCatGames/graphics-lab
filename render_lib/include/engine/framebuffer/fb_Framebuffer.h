#pragma once
#include <algorithm>
#include <string_view>
#include <vector>
#include <utility>

#include <engine/eng_vec.h>

namespace eng::fb {

class Framebuffer {
public:
	constexpr Framebuffer();
	constexpr Framebuffer(const int width, const int height);

	constexpr void SetPixelColor(const int x, const int y, const Vector3D<float> &v) { m_fb[XyToIndex(x, y)] = v;}
	constexpr void SetPixelColor(const int index, const Vector3D<float> &v) { m_fb[index] = v; }

	constexpr const Vector3D<float>& GetPixelColor(const int x, const int y) const { return m_fb[XyToIndex(x, y)]; }
	constexpr const Vector3D<float>& GetPixelColor(const int index) const { return m_fb[index]; }

	constexpr auto GetWidth() const { return m_width; }
	constexpr auto GetHeight() const { return m_height; }
	constexpr auto GetDimensions() const { return std::pair{m_width, m_height}; }

	constexpr void FillColor(const Vector3D<float> &v) { std::ranges::fill(m_fb, v); }
private:
	static constexpr int CalcSize(const int width, const int height) { return width * height; }

	constexpr int XyToIndex(const int x, const int y) const { return y * m_width + x; }
	constexpr auto IndexToXy(const int idx) const {
		const auto x = idx % m_width;
		const auto y = idx / m_width;
		return std::pair {x, y};
	}
private:
	int m_width, m_height;
	std::vector<Vector3D<float>> m_fb;
}; // class Framebuffer

constexpr Framebuffer::Framebuffer() : m_width(100), m_height(100), m_fb(CalcSize(100, 100)) {}

constexpr Framebuffer::Framebuffer(const int width, const int height) :
	m_width(width), m_height(height), m_fb(CalcSize(width, height)) {}

} // namespace eng::fb
