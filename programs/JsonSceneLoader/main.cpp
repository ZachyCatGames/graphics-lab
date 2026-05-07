#include <engine/eng_Engine.h>
#include <engine/eng_HandleGraphicsArgs.h>
#include <engine/eng_ObjectBase.h>
#include <engine/eng_Scene.h>
#include <engine/framebuffer/fb_PngWriter.h>

using namespace eng;

int main(int argc, char** argv) {
    /* Setup the core engine context. */
    Engine engine;
    engine.Initialize(argc, argv);

    /* Load the scene. */
    const GraphicsArgs& args = engine.GetArguments();
    std::shared_ptr<Scene> pScene = engine.LoadSceneFromJson(args.inputFileName);

    /* Grab main camera. */
    Handle<ICamera> camera = pScene->cameras.Get("main");

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
