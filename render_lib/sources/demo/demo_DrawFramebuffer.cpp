#include <engine/demo/demo_DrawFramebuffer.h>

#ifdef __SWITCH__
#include <engine/framebuffer/nx/fb_NativeWindowWriter.plat-nx.h>
#else
#include <engine/framebuffer/fb_PngWriter.h>
#endif

namespace eng::demo {

void DrawFramebuffer(const fb::Framebuffer& fb) {
    /*
     * Draw out the framebuffer.
     * Usually we'll write our framebuffer out to a PNG file.
     * But on NX we instead write our framebuffer to the screen through native
     * windows then loop until the user presses plus.
     */
#ifndef __SWITCH__
    fb::PngWriter png_writer("test.png");
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

} // namespace eng::demo
