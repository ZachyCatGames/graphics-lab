#include <engine/eng_HandleGraphicsArgs.h>
#include <engine/eng_ObjectBase.h>
#include <engine/eng_Scene.h>
#include <engine/eng_SceneParser_JSON.h>
#include <engine/eng_SceneLoader.h>
#include <engine/eng_ThreadedRenderer.h>

#include <engine/demo/demo_DrawFramebuffer.h>

#include <array>

using namespace eng;

int main(int argc, char** argv) {
    /* Parse arguments. */
    GraphicsArgs args;
    args.process(argc, argv);

    /* Create our scene. */
    Scene scene(args.recursionDepth);

    /* Create our renderer. */
    ThreadedRenderer renderer(&scene, args.width, args.height, args.rpp, args.randpix, args.numCpus);

    /* Setup our scene with objects from the specified JSON. */
    auto scene_loader = std::make_shared<SceneLoader>(&renderer, &scene);
    SceneParser_JSON parser(scene_loader);
    parser.parseFileData(args.inputFileName);

    /* Render the scene to a framebuffer. */
    fb::Framebuffer fb(args.width, args.height);
    renderer.Render(0, &fb);

    /* Write it out to a file. */
    demo::DrawFramebuffer(fb);
}
