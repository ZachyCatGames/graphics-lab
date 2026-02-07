#include "PerspectiveCamera.h"
#include "framebuffer/Framebuffer.h"
#include "framebuffer/PngWriter.h"

#include <print>
#include <cstdio>

#ifdef __SWITCH__
#include "framebuffer/nx/NativeWindowWriter.plat-nx.h"
#endif

extern "C" int main(int argc, char** argv) {
    int img_width  = 1280;
    int img_height = 720;

    auto aspect_ratio = static_cast<float>(img_width) / img_height;

    float vp_height = 0.5;
    float vp_width  = 0.5;

    eng::Vector3DF pos{3, -4, 12};
    eng::Vector3DF dir{69, 420, 64};
    float foc_len = 0.01;
    
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

#ifndef __SWITCH__
    eng::PngWriter png_writer("test.png");
    png_writer.WriteFramebuffer(fb);
#else
    // Configure our supported input layout: a single player with standard controller styles
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    // Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    PadState pad;
    padInitializeDefault(&pad);


    ::NWindow* nwin = ::nwindowGetDefault();

    /* Create our framebuffer. */
    ::Framebuffer native_fb;
    ::framebufferCreate(&native_fb, nwin, 1280, 720, PIXEL_FORMAT_RGBA_8888, 2);
    ::framebufferMakeLinear(&native_fb);

    /* Create NativeWindow fb writer. */
    eng::plat::nx::NativeWindowWriter writer(&native_fb);
    writer.WriteFramebuffer(fb);

    // Main loop
    while (appletMainLoop()) {
        // Scan the gamepad. This should be done once for each frame
        padUpdate(&pad);

        // padGetButtonsDown returns the set of buttons that have been
        // newly pressed in this frame compared to the previous one
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus)
            break; // break in order to return to hbmenu

    }

    /* Finalize the native framebuffer. */
    ::framebufferClose(&native_fb);

#endif
}
