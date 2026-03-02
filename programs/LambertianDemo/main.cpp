#include <engine/eng_ObjectBase.h>
#include <engine/eng_Scene.h>

#include <engine/demo/demo_SceneRenderHelper.h>

#include <engine/shape/shape_Sphere.h>
#include <engine/shape/shape_Triangle.h>

#include <engine/shader/shdr_NormalShader.h>
#include <engine/shader/shdr_Diffuse.h>
#include <engine/shader/shdr_Lambertian.h>
#include <engine/shader/shdr_BlinnPhong.h>
#include <engine/shader/shdr_Mirror.h>
#include <engine/shader/shdr_FlatColorShader.h>
#include <engine/shader/shdr_Emitter.h>

#include <array>
#include <print>
#include <cstdio>
#include <memory>
#include <thread>
#include <vector>

#ifdef __SWITCH__
#include "framebuffer/nx/NativeWindowWriter.plat-nx.h"
#endif

using namespace eng;

#define VECTOR_COLOR_IMG

int main(int argc, char** argv) {
    /* Initialize the demo helper. */
    demo::SceneRenderHelper helper(argc, argv);

    /* Get the scene from the helper. */
    Scene* p_scene = helper.GetScene();

    static constexpr auto lights = std::to_array<shdr::PointLight>(
        {
            { Vector3DF(-10, 20, 0), Vector3DF(1,1,1) }
        }
    );

    /* Create gray ground shader. */
    auto gray_shader = shdr::Lambertian::Create(
        shdr::FlatColor::Create(Vector3DF(0.75, 0.75, 0.75)),
        lights
    );

    /* Create purple ball shader. */
    auto ball_shader = shdr::Lambertian::Create(
        shdr::FlatColor::Create(Vector3DF(1, 0, 1)), lights
    );

    /* Create color shaders. */
    auto red_shader = shdr::Lambertian::Create(
        shdr::FlatColor::Create(Vector3DF(1, 0, 0)), lights
    );
    auto green_shader = shdr::Lambertian::Create(
        shdr::FlatColor::Create(Vector3DF(0, 1, 0)), lights
    );
    auto blue_shader = shdr::Lambertian::Create(
        shdr::FlatColor::Create(Vector3DF(0, 0, 1)), lights
    );

    static constexpr float TriBaseX = -7.0;
    static constexpr float TriBaseY = 22.0;
    static constexpr float TriBaseZ = -30.0;

    /* Create The Triforce. */
    p_scene->EmplaceShape<shape::Triangle>(
        Vector3DF(TriBaseX-10, TriBaseY, TriBaseZ), Vector3DF(TriBaseX, TriBaseY, TriBaseZ), Vector3DF(TriBaseX-5, TriBaseY+10, TriBaseZ)
    ).BindShader(blue_shader);
    p_scene->EmplaceShape<shape::Triangle>(
        Vector3DF(TriBaseX, TriBaseY, TriBaseZ), Vector3DF(TriBaseX+10, TriBaseY, TriBaseZ), Vector3DF(TriBaseX+5, TriBaseY+10, TriBaseZ)
    ).BindShader(green_shader);
    p_scene->EmplaceShape<shape::Triangle>(
        Vector3DF(TriBaseX-5, TriBaseY+10, TriBaseZ), Vector3DF(TriBaseX+5, TriBaseY+10, TriBaseZ), Vector3DF(TriBaseX, TriBaseY+20, TriBaseZ)
    ).BindShader(red_shader);

    /* Create ground plane. */
    p_scene->EmplaceShape<shape::Triangle>(
        Vector3DF(-200, 0, 50), Vector3DF(200, 0, 50), Vector3DF(0, 0, -2000)
    ).BindShader(gray_shader);

    /* Make a sphere. */
    p_scene->EmplaceShape<eng::shape::Sphere>(eng::Vector3DF{4, 6, -10}, 5)
        .BindShader(ball_shader);

    /* And another one. */
    p_scene->EmplaceShape<eng::shape::Sphere>(eng::Vector3DF{-9, 6, -14}, 5)
        .BindShader(shdr::Mirror::Create());

    /* Render the scene. */
    helper.RenderScene();
    helper.DrawFramebuffer();
}
