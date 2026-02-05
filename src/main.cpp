#include "PerspectiveCamera.h"
#include "Framebuffer.h"

int main() {
    int img_width  = 1280;
    int img_height = 720;

    auto aspect_ratio = static_cast<float>(img_width) / img_height;

    float vp_height = 0.5;
    float vp_width  = 0.5;

    eng::Vector3DF pos{3, -4, 12};
    eng::Vector3DF dir{-8, -5, 10};
    float foc_len = 0.001;
    
    eng::PerspectiveCamera cam(pos, dir, foc_len, img_width, img_height, vp_width);
    eng::Framebuffer fb(img_width, img_height);

    for (int y = 0; y < img_height; y++) {
        for (int x = 0; x < img_width; x++) {
            auto r = cam.GenerateRay(x, y);

            auto color = static_cast<eng::Vector3DF>((r.direction().normalize() / 2.0) + 0.5);
            fb.SetPixelColor(x, y, color);
            //std::cout << (r.direction()) << '\n';
        }
    }

    fb.ExportToPng("test.png");
}
