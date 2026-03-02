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

    /* Create our light. */
    shdr::PointLight light(Vector3DF(-10, 20, 0), Vector3DF(1,1,1));

    /* Create color shader. */
    auto color_shader = shdr::FlatColor::Create(Vector3DF(0.75, 0.75, 0.75));

    //auto shader = eng::BlinnPhongShader::Create(eng::ray{ eng::Vector3DF{ 0,0,0 }, eng::Vector3DF(3, 4,5)}, 20.0);
    //auto shader = eng::shdr::Lambertian::Create(eng::Ray{ eng::Vector3DF{ 0,0,0 }, eng::Vector3DF(3, 4,5)});
    /*
    p_scene->EmplaceShape<eng::shape::Triangle>(eng::Vector3DF{-4.426795, 1.13923, -7}, eng::Vector3DF{-4.833013, -0.44282, -5}, eng::Vector3DF{-4.45, -0.779423, -5});
    p_scene->EmplaceShape<shape::Sphere>(Vector3DF(-5, -5, -10), 2)
        .BindShader(shdr::Mirror::Create());
    */
    /* Create ground plane. */
    p_scene->EmplaceShape<shape::Triangle>(
        Vector3DF(-200, 0, 50), Vector3DF(200, 0, 50), Vector3DF(0, 0, -2000)
    ).BindShader(shdr::Lambertian::Create(color_shader, std::array{
        light
    }));

    /* Make a sphere. */
    p_scene->EmplaceShape<eng::shape::Sphere>(eng::Vector3DF{3, 6, -10}, 5)
        .BindShader(shdr::Lambertian::Create(
            color_shader,
            std::array {
                light
            }
    ));

    /* And another one. */
    p_scene->EmplaceShape<eng::shape::Sphere>(eng::Vector3DF{-9, 6, -14}, 5)
        .BindShader(shdr::Mirror::Create());

    /* Render the scene. */
    helper.RenderScene();
    helper.DrawFramebuffer();
}
