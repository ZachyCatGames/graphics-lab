#include <engine/eng_PerspectiveCamera.h>
#include <engine/framebuffer/fb_Framebuffer.h>

#include <engine/demo/demo_Arguments.h>
#include <engine/demo/demo_DrawFramebuffer.h>

using namespace eng;

#define VECTOR_COLOR_IMG

int main(int argc, char** argv) {
    /* Parse arguments. */
    GraphicsArgs args;
    auto [argc_real, argv_real] = demo::GetArguments(argc, argv);
    args.process(argc_real, argv_real);

    const auto img_width = args.width;
    const auto img_height = args.height;

    float vp_height = 2.0;
    float vp_width  = 0.5;

    Vector3DF pos{13,-7,12};
    Vector3DF dir{5,18,2};
    float foc_len = 0.001;

    PerspectiveCamera cam(pos, dir, foc_len, img_width, img_height, vp_width);
    
    fb::Framebuffer fb(img_width, img_height);

    for (int y = 0; y < img_height; y++) {
        for (int x = 0; x < img_width; x++) {
            auto r = cam.GenerateRay(x, y);

            auto color = static_cast<eng::Vector3DF>((r.direction().normalize() / 2.0) + 0.5);
            fb.SetPixelColor(x, y, color);
            //std::cout << (r.direction()) << '\n';
        }
    }

    /* Draw out the framebuffer. */
    demo::DrawFramebuffer(fb);

}
