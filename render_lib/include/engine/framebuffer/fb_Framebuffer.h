#pragma once
#include <algorithm>
#include <string_view>
#include <vector>
#include <utility>

#include <engine/eng_Vector3D.h>

namespace eng::fb {

class Framebuffer {
public:
	constexpr Framebuffer();
	constexpr Framebuffer(const int width, const int height);

	constexpr void SetPixelColor(const int x, const int y, const Vector3D<float> &v) {
		assert(x >= 0 && x < m_width);
		assert(y >= 0 && x < m_height);
		m_fb[XyToIndex(x, y)] = v;
	}
	constexpr void SetPixelColor(const int index, const Vector3D<float> &v) {
		assert(index >= 0 && index < m_width * m_height);
		m_fb[index] = v;
	}

	[[nodiscard]] constexpr const Vector3D<float>& GetPixelColor(const int x, const int y) const {
		assert(x >= 0 && x < m_width);
		assert(y >= 0 && x < m_height);
		return m_fb[XyToIndex(x, y)];
	}
	[[nodiscard]] constexpr const Vector3D<float>& GetPixelColor(const int index) const {
		assert(index >= 0 && index < m_width * m_height);
		return m_fb[index];
	}

	[[nodiscard]] constexpr auto GetWidth() const { return m_width; }
	[[nodiscard]] constexpr auto GetHeight() const { return m_height; }
	[[nodiscard]] constexpr auto GetDimensions() const { return std::pair{m_width, m_height}; }

	constexpr void FillColor(const Vector3D<float> &v) { std::ranges::fill(m_fb, v); }
private:
	[[nodiscard]] static constexpr int CalcSize(const int width, const int height) { return width * height; }

	[[nodiscard]] constexpr int XyToIndex(const int x, const int y) const { return y * m_width + x; }
	[[nodiscard]] constexpr auto IndexToXy(const int idx) const {
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
