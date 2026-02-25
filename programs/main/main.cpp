#include <engine/eng_ObjectBase.h>
#include <engine/eng_PerspectiveCamera.h>
#include <engine/eng_Scene.h>

#include <engine/framebuffer/fb_Framebuffer.h>
#include <engine/framebuffer/fb_PngWriter.h>

#include <engine/shape/shape_Sphere.h>
#include <engine/shape/shape_Triangle.h>

#include <engine/shader/shdr_NormalShader.h>
#include <engine/shader/shdr_Lambertian.h>
#include <engine/shader/shdr_BlinnPhong.h>
#include <engine/shader/shdr_Mirror.h>
#include <engine/shader/shdr_FlatColorShader.h>

#include <print>
#include <cstdio>
#include <memory>
#include <vector>

#ifdef __SWITCH__
#include "framebuffer/nx/NativeWindowWriter.plat-nx.h"
#endif

using namespace eng;

#define VECTOR_COLOR_IMG

int main(int argc, char** argv) {
    int img_width  = 200;
    int img_height = 200;

    auto aspect_ratio = static_cast<float>(img_width) / img_height;

#if 1
    float vp_height = 2.0;
    float vp_width  = 0.5;

    eng::Vector3DF pos{0, 0, 0};
    eng::Vector3DF dir{0.0, 0, -1.0};
    dir = dir.normalize();
    float foc_len = 0.2;

    eng::PerspectiveCamera cam(pos, dir, foc_len, img_width, img_height, vp_width);
    eng::fb::Framebuffer fb(img_width, img_height);

    //auto shader = eng::BlinnPhongShader::Create(eng::ray{ eng::Vector3DF{ 0,0,0 }, eng::Vector3DF(3, 4,5)}, 20.0);
    //auto shader = eng::shdr::Lambertian::Create(eng::Ray{ eng::Vector3DF{ 0,0,0 }, eng::Vector3DF(3, 4,5)});

    eng::Scene scene;
    /*
    scene.EmplaceShape<eng::shape::Triangle>(eng::Vector3DF{-4.426795, 1.13923, -7}, eng::Vector3DF{-4.833013, -0.44282, -5}, eng::Vector3DF{-4.45, -0.779423, -5});
    scene.EmplaceShape<shape::Sphere>(Vector3DF(-5, -5, -10), 2)
        .BindShader(shdr::Mirror::Create());
    */
    /* Create ground plane. */
    scene.EmplaceShape<shape::Triangle>(eng::Vector3DF{-4.426795, 1.13923, -7}, eng::Vector3DF{-4.833013, -0.44282, -5}, eng::Vector3DF{-4.45, -0.779423, -5});

    /*
    scene.EmplaceShape<shape::Triangle>(
        Vector3DF(200, 0, 50), Vector3DF(-200, 0, 50), Vector3DF(0, 0, -2000)
    )
   .BindShader(shdr::Lambertian::Create(nullptr, Vector3DF(0, 20, 0), Vector3DF(0.5, 0.5, 0.5)));
    */ 

    /* Make a sphere. */
    scene.EmplaceShape<eng::shape::Sphere>(eng::Vector3DF{0, 6, -10}, 5)
        .BindShader(shdr::Lambertian::Create(
            shdr::FlatColor::Create(Vector3DF(1,0,0)),
            Vector3DF(10, 0, 0),
            Vector3DF(1,1,1))
    );

    /* And another one. */
    scene.EmplaceShape<eng::shape::Sphere>(eng::Vector3DF{-9, 6, -14}, 4)
        .BindShader(shdr::Lambertian::Create(
            shdr::FlatColor::Create(Vector3DF(1,0,0)),
            Vector3DF(10, 0, 0),
            Vector3DF(1,1,1))
    );
    //scene.EmplaceShape<eng::shape::Sphere>(eng::Vector3DF{0, 0, -14}, 4)
    //    .BindShader(shdr::Normal::Create()
    //);
    scene.EmplaceShape<eng::shape::Sphere>(eng::Vector3DF{-9, 6, -14}, 5)
        .BindShader(shdr::Mirror::Create()
    );

    //eng::Sphere sphere({0, 0, -4}, 4.955);
    for (int y = 0; y < img_height; y++) {
        for (int x = 0; x < img_width; x++) {
            auto color = scene.GetPixelColor(&cam, x, y);

            fb.SetPixelColor(x, y, color);
        }
    }
#endif

#if 0

    float vp_height = 2.0;
    float vp_width  = 0.5;

    eng::Vector3DF pos{13,-7,12};
    eng::Vector3DF dir{5,18,2};
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
#endif // VECTOR_COLOR_IMG

#ifndef __SWITCH__
    eng::fb::PngWriter png_writer("test.png");
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
