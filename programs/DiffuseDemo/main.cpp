#include "engine/eng_Handle.h"
#include "engine/eng_HandleGraphicsArgs.h"
#include "engine/eng_IShader.h"
#include "engine/eng_Vector3D.h"
#include <engine/eng_Scene.h>

#include <engine/eng_Engine.h>
#include <engine/framebuffer/fb_PngWriter.h>

#include <array>

using namespace eng;


constexpr int img_width  = 2000;
constexpr int img_height = 2000;

    constexpr float vp_height = 2.0;
    constexpr float vp_width  = 0.5;

    constexpr eng::Vector3DF pos{0, 5, 0};
    constexpr eng::Vector3DF dir = Vector3DF{0.0, 0, -1.0};
    constexpr float foc_len = 0.2;

    constexpr auto aspect_ratio = static_cast<float>(img_width) / img_height;

fb::Framebuffer fbb(img_width, img_height);

int main(int argc, char** argv) {

    Engine engine;
    engine.Initialize(argc, argv);
    const GraphicsArgs& args = engine.GetArguments();

    auto scene = engine.GetActiveScene();

    /* Shader setup. */
    Handle<IShader> gndShader = engine.CreateDiffuseShader({
        .diffuse = Vector3DF(0.75, 0.75, 0.75)
    });

    Handle<IShader> redShader = engine.CreateEmitter({
        .ambientLight = Vector3DF(55,0,0)
    });
    Handle<IShader> greenShader = engine.CreateEmitter({
        .ambientLight = Vector3DF(0,55,0)
    });
    Handle<IShader> blueShader = engine.CreateEmitter({
        .ambientLight = Vector3DF(0,0,55)
    });

    scene->shaders.Insert("groundShader", gndShader);
    scene->shaders.Insert("red", redShader);
    scene->shaders.Insert("green", greenShader);
    scene->shaders.Insert("blue", blueShader);

    /* Create ground plane. */
    auto ground = engine.CreateTriangle(Vector3DF(-200, 0, 50), Vector3DF(200, 0, 50), Vector3DF(0, 0, -2000), gndShader);
    scene->shapes.Insert("ground", ground);

    static constexpr float TriBaseX = -7.0;
    static constexpr float TriBaseY = 22.0;
    static constexpr float TriBaseZ = -30;

    /* Create The Triforce. */
    auto tri1 = engine.CreateTriangle(
        Vector3DF(TriBaseX-10, TriBaseY, TriBaseZ), Vector3DF(TriBaseX, TriBaseY, TriBaseZ), Vector3DF(TriBaseX-5, TriBaseY+10, TriBaseZ),
        redShader
    );
    auto tri2 = engine.CreateTriangle(
        Vector3DF(TriBaseX, TriBaseY, TriBaseZ), Vector3DF(TriBaseX+10, TriBaseY, TriBaseZ), Vector3DF(TriBaseX+5, TriBaseY+10, TriBaseZ),
        greenShader
    );
    auto tri3 = engine.CreateTriangle(
        Vector3DF(TriBaseX-5, TriBaseY+10, TriBaseZ), Vector3DF(TriBaseX+5, TriBaseY+10, TriBaseZ), Vector3DF(TriBaseX, TriBaseY+20, TriBaseZ),
        blueShader
    );
    scene->shapes.Insert("tri1", tri1);
    scene->shapes.Insert("tri2", tri2);
    scene->shapes.Insert("tri3", tri3);

    auto emitter = engine.CreateSphere(Vector3DF(0, 30, -10), 5,
        engine.CreateEmitter({
            .ambientLight = Vector3DF(7, 7, 7)
        })
    );
    scene->shapes.Insert("light", emitter);

    /* Create a sphere. */
    auto sphere = engine.CreateSphere(eng::Vector3DF{4, 6, -10}, 5,
        engine.CreateDiffuseShader({
            .diffuse = Vector3DF(1,0,1)
        })
    );

    /* Create another.*/
    auto sphere2 = engine.CreateSphere(eng::Vector3DF{-9, 6, -14}, 5,
        engine.CreateMirror()
    );
    scene->shapes.Insert("sphere1", sphere);
    scene->shapes.Insert("sphere2", sphere2);

    Handle<ICamera> camera = engine.CreatePerspectiveCamera(pos, dir, foc_len, args.width, args.height, vp_width);
    assert(camera);
    scene->cameras.Insert("main", camera);

    Handle<RenderBuffer> renderBuffer = nullptr;
    std::string renderString = engine.GetRenderString();
    if (renderString == "opengl") {
        renderBuffer = engine.GetDisplayRenderBuffer();
    } else {
        renderBuffer = engine.CreateExportableRenderBuffer(args.width, args.height);
    }
    assert(renderBuffer.IsValid());

    if (renderString == "opengl") {
        GLFWwindow* window = engine.GetGlfwWindow();

        /* Get image dims. */
        size_t winWidth  = renderBuffer->GetWidth();
        size_t winHeight = renderBuffer->GetHeight();

        glm::mat4 modelMatrix, normalMatrix;
        constexpr float sensitivity = 0.2f;
        float rotationAngle = 0;
        float lastX = winWidth / 2.0f;
        float lastY = winHeight / 2.0f;
        float yaw = 0;
        float pitch = 0;
        while (!glfwWindowShouldClose(window))
        {
            // Clear the window's buffer (or clear the screen to our
            // background color)
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            /* Increment rotation angle. */
            //mesh->IncrementRotationAngle(0.05);

            /* Render your objects here */
            /* (my amazing triangle) */
            engine.RenderActiveScene("main", renderBuffer);

            // Swap the front and back buffers
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();

            /* Check for movement inputs. */
            constexpr float moveRatePerFrame = 0.05;
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                camera->MoveByW(-moveRatePerFrame);
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                camera->MoveByU(-moveRatePerFrame);
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                camera->MoveByW(moveRatePerFrame);
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                camera->MoveByU(moveRatePerFrame);
            }

            /* Get mouse position. */
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            /* Calculate diffs + update previous pos. */
            float xDiff = xpos - lastX;
            float yDiff = ypos - lastY;
            lastX = xpos;
            lastY = ypos;

            /* Increment and clamp yaw & pitch. */
            yaw += xDiff * sensitivity;
            pitch += yDiff * sensitivity;
            if (pitch > 89.9f)
                pitch = 89.9f;
            else if (pitch < -89.9f)
                pitch = -89.9f;

            //std::cout << xpos << ", " << ypos << '\n';

            camera->Rotate(pitch, yaw);

            if (glfwGetKey( window, GLFW_KEY_T ) == GLFW_PRESS) {
                //std::cout << "fps: " << 1.0/timeDiff << std::endl;
            }
            if (glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, 1);
            }
        }
    } else {
        engine.RenderActiveScene("main", renderBuffer);
        auto fb = renderBuffer.StaticCast<ExportableRenderBuffer>()->ExportToFramebuffer();
        fb::PngWriter fbWriter("out.png");
        fbWriter.WriteFramebuffer(fb);
    }
}
