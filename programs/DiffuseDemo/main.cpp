#include <engine/eng_ObjectBase.h>
#include <engine/eng_Scene.h>

#include <engine/demo/demo_SceneRenderHelper.h>

#include <engine/shape/shape_Sphere.h>
#include <engine/shape/shape_Triangle.h>

#include <engine/shader/shdr_Diffuse.h>
#include <engine/shader/shdr_Mirror.h>
#include <engine/shader/shdr_FlatColorShader.h>
#include <engine/shader/shdr_Emitter.h>

#include <array>

using namespace eng;

int main(int argc, char** argv) {
    /* Initialize the demo helper. */
    demo::SceneRenderHelper helper(argc, argv);

    /* Get the scene from the helper. */
    Scene* p_scene = helper.GetScene();

    /* Create ground plane. */
    p_scene->EmplaceShape<shape::Triangle>(
        Vector3DF(-200, 0, 50), Vector3DF(200, 0, 50), Vector3DF(0, 0, -2000)
    ).BindShader(shdr::Diffuse::Create(
        shdr::FlatColor::Create(Vector3DF(0.75, 0.75, 0.75))
    ));

    /* Create a light emitter. */
    p_scene->EmplaceShape<shape::Sphere>(Vector3DF(0, 30, -10), 5)
        .BindShader(shdr::Emitter::Create(Vector3DF(30, 30, 30)));

    /* Make a sphere. */
    p_scene->EmplaceShape<eng::shape::Sphere>(eng::Vector3DF{3, 6, -10}, 5)
        .BindShader(shdr::Diffuse::Create(
            shdr::FlatColor::Create(Vector3DF(0.0, 0.75, 0.0))
    ));

    /* And another one. */
    p_scene->EmplaceShape<eng::shape::Sphere>(eng::Vector3DF{-9, 6, -14}, 5)
        .BindShader(shdr::Mirror::Create());

    /* Render the scene. */
    helper.RenderScene();
    helper.DrawFramebuffer();
}
